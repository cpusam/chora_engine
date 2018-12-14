#include "widget.hpp"
#include <algorithm>

void Widget::destroy (  )
{
	for (auto * it: child)
		if (it)
		{
			it->destroy();
			delete it;
		}
	child.clear();
}

void Widget::setID ( std::string new_id )
{
	id = new_id;
}

std::string Widget::getID (  )
{
	return id;
}

void Widget::setPosition ( Vect p )
{
	if (position.x == p.x && position.y == p.y)
		return;

	position = p;
	dim.x = int(p.x), dim.y = int(p.y);

	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->setPosition(Vect::add((*i)->getRelativePosition(), position));
}

Vect Widget::getPosition (  )
{
	return position;
}

void Widget::setRelativePosition ( Vect p )
{
	if (rel_pos.x == p.x && rel_pos.y == p.y)
		return;

	rel_pos = p;

	if (parent)
	{
		position.x = parent->getPosition().x + rel_pos.x;
		position.y = parent->getPosition().y + rel_pos.y;
		dim.x = int(position.x);
		dim.y = int(position.y);
	}
	else
	{
		dim.x = int(rel_pos.x);
		dim.y = int(rel_pos.y);
		position = rel_pos;
	}

	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		(*i)->setPosition((*i)->getRelativePosition() + position);
}

Vect Widget::getRelativePosition (  )
{
	return rel_pos;
}

void Widget::setDimension ( SDL_Rect d )
{
	d.x = int(position.x + rel_pos.x);
	d.y = int(position.y + rel_pos.y);
	dim = d;
}

SDL_Rect Widget::getDimension (  )
{
	return dim;
}

void Widget::show ( bool s )
{
	visible = s;
	showChild(s);
}

bool Widget::isVisible (  )
{
	return visible;
}

void Widget::setParent ( Widget * w )
{
	if (parent)
		parent->removeChild(this);
	parent = w;
}

Widget * Widget::getParent (  )
{
	return parent;
}

bool Widget::hasChild ( Widget * w )
{
	for (unsigned int i(0); i < child.size(); i++)
		if (w == child[i] && w)
			return true;

	return false;
}

void Widget::showChild ( bool s )
{
	if (visible)
		for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		{
			(*i)->show(s);
			(*i)->showChild(s);
		}
}

bool Widget::addChild ( Widget * w )
{
	if (!hasChild(w))
	{
		w->setParent(this);
		child.push_back(w);
		return true;
	}

	return false;
}

void Widget::clearChildren (  )
{
	child.clear();
}

bool Widget::removeChild ( Widget * w )
{
	if (hasChild(w))
	{
		for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
			if (*i == w)
			{
				child.erase(i);
				return true;
			}
	}

	return false;
}

Widget * Widget::getChild ( std::string s_id )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->getID() == s_id)
			return (*i);

	return 0;
}

Widget * Widget::getChild ( int index )
{
	if (index > -1 && index < (int)child.size())
		return child.at(index);

	return 0;
}

const std::vector<Widget *> Widget::getAllChildren()
{
	std::vector<Widget *> all = this->child;
	
	for (size_t i = 0, end = all.size(); i < end; ++i)
	{
		std::vector<Widget *> children = all[i]->getChildren();
		for (Widget * child: children)
			if (child && std::find(all.begin(), all.end(), child) == all.end())
				all.push_back(child);
	}
	
	return all;
}

const std::vector<Widget *> & Widget::getChildren (  )
{
	return child;
}

/*
	as funções childInput, childUpdate e childDraw devem ser chamadas nas
	respectivas funções input, update e draw isso se a classe derivada
	de Widget possuir filhos.
*/
void Widget::childInput ( SDL_Event & event )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->isVisible() && visible)
			(*i)->input(event);
}

int Widget::childUpdate (  )
{
	for (std::vector <Widget *>::iterator i = child.begin(); i != child.end(); i++)
		if ((*i)->isVisible() && visible)
			(*i)->update();

	return DEFAULT_STATE;
}


void Widget::childDraw ( SDL_Renderer * renderer )
{
	for (std::vector <Widget *>::iterator i = child.begin(), end = child.end(); i != end; i++)
		if ((*i)->isVisible() && visible)
			(*i)->draw(renderer);
}


void Widget::input ( SDL_Event & event )
{
	childInput(event);
}

int Widget::update (  )
{
	childUpdate();

	return getState();
}

void Widget::draw ( SDL_Renderer * renderer )
{
	childDraw(renderer);
}


