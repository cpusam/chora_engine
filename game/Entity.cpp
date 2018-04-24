#include "Entity.hpp"
#include "../Exception.hpp"
#include <math.h>

unsigned long int Entity::countID = 1;

Entity::Entity()
{
	layer = 0;
	id = countID;
	countID++;
	std::cout<<"ID="<<id<<std::endl;
	dir = NONE_DIR;
	visible = true;

	level = nullptr;
	collPoints = 3;
	topLadderTile = -1;
	topTileSize = 10;//mesmo valor que velMax;
	collRect = (SDL_Rect){0,0,0,0};//retangulo de colisão
	texture = nullptr;
	currAnim = nullptr;
	name = "";
}

Entity::~Entity()
{

}

int Entity::getLayer (  )
{
	return layer;
}

void Entity::setLayer ( int layer )
{
	this->layer = layer;
}

void Entity::setName ( std::string n )
{
	name = n;
}

std::string Entity::getName (  )
{
	return name;
}

void Entity::setGroup ( std::string g )
{
	group = g;
}

std::string Entity::getGroup (  )
{
	return group;
}

unsigned long int Entity::getId (  )
{
	return id;
}

void Entity::show ( bool s )
{
	visible = s;
}

bool Entity::isVisible (  )
{
	return visible;
}

void Entity::setDir ( Direction d )
{
	dir = d;
}

Direction Entity::getDir (  )
{
	return dir;
}

Animation * Entity::getCurrAnim (  )
{
	return currAnim;
}

bool Entity::setCurrAnim ( std::string animName )
{
	std::map<std::string, Animation>::const_iterator it = anim.find(animName);

	if (it != anim.end())
	{
		currAnim = &anim[animName];
		return true;
	}

	return false;
}

void Entity::changeAnim ( std::string animName, bool reset )
{
	if (anim.size() > 0)
	{
		if (anim.find(animName) != anim.end())
		{
			currAnim = &anim[animName];
		}
		else
			std::cout<<"Entity::Não encontrou uma animação chamada "<<animName<<std::endl;
	}

	if (reset)
		currAnim->reset();
}

Vect Entity::getPosition (  )
{
	return pos;
}

void Entity::setPosition ( Vect p )
{
	pos = p;
}

void Entity::setTopLadderTile ( int t )
{
	topLadderTile = t;
}

int Entity::getTopLadderTile (  )
{
	return topLadderTile;
}

void Entity::changeDir ( Direction d )
{
	bool changeH = true, changeV = true;

	if (dir == d)
		return;


	changeH = (d & RIGHT_DIR)||(d & LEFT_DIR);
	changeV = (d & UP_DIR)||(d & DOWN_DIR);

	for (std::map<std::string,Animation>::iterator it = anim.begin(); it != anim.end(); it++)
		it->second.flip(changeH, changeV);

	dir = d;
}

bool Entity::receive ( Entity * sender, std::string mesg )
{
	return false;
}

TileMap * Entity::getLevel (  )
{
	return level;
}

void Entity::setLevel ( TileMap * level )
{
	this->level = level;
}

Vect Entity::getCollCenter (  )
{
	return Vect(int(pos.x)+collRect.x+collRect.w/2,int(pos.y)+collRect.y+collRect.h/2);
}

SDL_Rect Entity::getCollRect (  )
{
	return (SDL_Rect){int(pos.x) + collRect.x, int(pos.y) + collRect.y, collRect.w,collRect.h};
}

void Entity::setCollRect ( SDL_Rect rect )
{
	collRect = rect; 
}

SDL_Rect Entity::getView (  )
{
	return (SDL_Rect){int(pos.x) + view.x, int(pos.y) + view.y, view.w,view.h};
}

bool Entity::isDir ( Direction d )
{
	return d & dir;
}

bool Entity::isSolidSide ( std::string side, int i )
{
	std::vector<Vect> vet;
	if (side == "up")
		vet = upSide;
	else if (side == "down")
		vet = downSide;
	else if (side == "right")
		vet = rightSide;
	else if (side == "left")
		vet = leftSide;
	else
	{
		std::cout<<"Error "<<side<<" lado não identificado\n";
		return false;
	}

	if (i >= 0)
	{
		if (i < int(vet.size()))
		{
			Vect p = vet.at(i);

			if (side == "up")
				p.y -= 1;
			else if (side == "down")
				p.y += 1;
			else if (side == "right")
				p.x += 1;
			else if (side == "left")
				p.x -= 1;

			return isSolid(p + pos);
		}

		return false;
	}

	for (auto & v: vet)
	{
		if (side == "up")
			v.y -= 1;
		else if (side == "down")
			v.y += 1;
		else if (side == "right")
			v.x += 1;
		else if (side == "left")
			v.x -= 1;

		if (isSolid(v + pos))
			return true;
	}

	return false;
}

std::vector<int>& Entity::getSolids (  )
{
	return solid;
}

void Entity::addSolid ( int s )
{
	solid.push_back(s);
}

bool Entity::remSolid ( int s )
{
	unsigned int bef = solid.size();
	std::vector<int> aux;
	for (auto i: solid)
		if (i != s)
		{
			aux.push_back(i);
		}

	solid = aux;
	return bef != solid.size();
}

void Entity::addOneWayUp ( int s )
{
	upSolid.push_back(s);
}

bool Entity::remOneWayUp ( int s )
{
	unsigned int bef = upSolid.size();
	std::vector<int> aux;
	for (auto i: upSolid)
		if (i != s)
		{
			aux.push_back(i);
		}

	upSolid = aux;
	return bef != upSolid.size();
}

bool Entity::isSolid ( Vect p )
{
	if (level == nullptr)
	{
		throw Exception("Entity::isSolid level é nulo");
	}

	int t = level->get_tile(int(p.x),int(p.y));

	for (auto tile: solid)
	{
		if (tile == t)
		{
			return true;
		}
	}

	return false;
}


bool Entity::isGround (  )
{
	if (level == nullptr)
		throw Exception("Entity::isGround level é nulo");

	setSides(collRect, collPoints);
	for (auto p: downSide)
	{
		p = p + pos;
		p.y += 1; // 1 pixel abaixo
		// verifica se é sólido
		if (isSolid(p))
			return true;
	}

	// meio da lateral de baixo do retangulo de colisão (collRect)
	int x1 = pos.x + collRect.x;
	int y1 = pos.y + collRect.y + collRect.h;
	// +1 para detectar o tile abaixo dele
	y1 = y1 + 1;

	int x2 = pos.x + collRect.x + collRect.w;
	int y2 = y1;

	int levelY = (y1/level->get_tilesize())*level->get_tilesize();
	//verifica se é o topo da escada na parte de baixo (downSide)
	if ((y1 - levelY) <= topTileSize && (level->get_tile(x1,y1) == topLadderTile || level->get_tile(x2,y2) == topLadderTile))
		if (vel.y == 0)
			return true;

	//colisão com plataforma
	if (vel.y == 0)
	{
		for (auto p: downSide)
		{
			p = p + pos;
			p.y += 1;

			for (auto tile: upSolid)
				if (tile == level->get_tile(p.x,p.y))
					return true;
		}
	}

	return false;
}

bool Entity::oneWayUpCollision ()
{
	Vect before = Vect(pos.x, pos.y);
	Vect after = before;

	if (vel.y <= 0)
		return false;

	before.y -= vel.y;

	for (auto p: downSide)
	{
		Vect q = p;
		Vect a = p + after;
		Vect b = p + before;

		bool jump = false;
		for (auto tile: upSolid)
			if (tile == level->get_tile(b.x,b.y))
			{
				jump = true;
				break;
			}


		if (jump)
		{
			for (auto tile: upSolid)
				if (tile == level->get_tile(a.x,a.y))
				{
					if (int(a.y)/level->get_tilesize() != int(b.y)/level->get_tilesize())
					{
						int y = int(a.y / level->get_tilesize()) * level->get_tilesize();
						pos.y = y - int(q.y) - 1;
						vel.y = 0;
						return true;
					}
				}
			continue;
		}


		p = a;
		for (auto tile: upSolid)
			if (tile == level->get_tile(p.x,p.y))
			{
				int y = int(p.y / level->get_tilesize()) * level->get_tilesize();
				pos.y = y - int(q.y) - 1;
				vel.y = 0;
				return true;
			}
	}

	return false;
}

bool Entity::collisionVer (  )
{
	if (level == nullptr)
		throw Exception("Entity::"+name+" collisionVer level map é nulo");
	if (vel.y == 0)
		return false;

	// atualiza a posição dos pontos de colisão
	setSides(collRect, collPoints);

	if (vel.y < 0)
	{
		//colisão acima da cabeça e embaixo do tile
		for (auto p: upSide)
		{
			p = p + pos;
			p.y += vel.y;
			p.y = round(p.y);
			if (isSolid(p))
			{
				int y = (int(p.y)/level->get_tilesize() + 1)*level->get_tilesize();
				pos.y = p.y - collRect.y + (y - int(p.y));
				pos.y = floor(pos.y);

				return true;
			}
		}
	}
	else
	{
		for (auto p: downSide)
		{
			p = p + pos;
			p.y += vel.y;
			p.y = round(p.y);
			if (isSolid(p))
			{
				int y = (int(p.y)/level->get_tilesize())*level->get_tilesize();
				pos.y = p.y - (collRect.y + collRect.h) + (y - int(p.y));
				pos.y = floor(pos.y) - 1;
				return true;
			}

			//tenta colidir com algum tile one way
			if (oneWayUpCollision())
				return true;
		}

		// colisão com a ponta da escada
		// aqui é colisão "oneway" do lado de cima
		for (auto p: downSide)
		{
			p = p + pos;
			p.y += vel.y;
			p.y = round(p.y);
			int x = p.x;
			int y = p.y;
			int levelY = (y / level->get_tilesize())*level->get_tilesize();
			if (y - levelY < topTileSize)
				if (level->get_tile(x,y) == topLadderTile)
				{
					pos.y = p.y - (collRect.y + collRect.h) + (levelY - int(p.y));
					pos.y = floor(pos.y) - 1;
					setSides(collRect,collPoints);
					return true;
				}
		}
	}

	return false;
}

bool Entity::collisionHor (  )
{
	if (level == nullptr)
		throw Exception("Entity::"+name+" collisionHor level map é nulo");
	if (vel.x == 0)
		return false;

	// atualiza a posição dos pontos de colisão
	setSides(collRect, collPoints);

	if (vel.x < 0)
	{
		for (auto p: leftSide)
		{
			p = p + pos;
			p.x += vel.x;
			p.x = round(p.x);
			if (isSolid(p))
			{
				int x = (int(p.x)/level->get_tilesize() + 1)*level->get_tilesize();
				pos.x = p.x -collRect.x + (x - int(p.x));
				pos.x = int(pos.x);
				setSides(collRect,collPoints);

				return true;
			}
		}
	}
	else
	{
		for (auto p: rightSide)
		{
			p = p + pos;
			p.x += vel.x;
			p.x = round(p.x);
			if (isSolid(p))
			{
				int x = (int(p.x)/level->get_tilesize())*level->get_tilesize();
				pos.x = p.x - (collRect.x + collRect.w) + (x - int(p.x));
				pos.x = int(pos.x) - 1;
				setSides(collRect,collPoints);
				return true;
			}
		}
	}

	return false;
}


void Entity::setSides ( SDL_Rect rect, int numPoints )
{
	float size, x, y;

	if (numPoints <= 2)
		throw Exception("Entity::"+name+" setSides numPoints <= 2");

	collPoints = numPoints;

	// limpa tudo
	rightSide.clear();
	leftSide.clear();
	upSide.clear();
	downSide.clear();

	size = rect.h/(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado direito
		x = rect.x + rect.w;
		y = rect.y + float(i) * size;
		rightSide.push_back(Vect(x,y));

		// define o lado esquerdo
		x = rect.x;
		//y = i * size;
		leftSide.push_back(Vect(x,y));
	}

	size = rect.w/(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado superior
		x = rect.x + i * size;
		y = rect.y;
		upSide.push_back(Vect(x,y));

		// define o lado inferior
		//x = 0;
		y = rect.y + rect.h;
		downSide.push_back(Vect(x,y));
	}

}

void Entity::drawSides ( SDL_Renderer * renderer, Camera * camera )
{
	SDL_Color color = {255,0,0,255};
	SDL_Rect rect = {0,0,5,5};

	for (auto p: rightSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	for (auto p: leftSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	color.r = 255;
	color.g = 255;
	for (auto p: downSide)
	{
		rect.x = p.x + pos.x;
		rect.y = p.y + pos.y;
		fill_rect(renderer, camera, color, rect);
	}

	// centro
	rect.x = pos.x + collRect.x + collRect.w/2;
	rect.y = pos.y + collRect.y + collRect.h/2;
	fill_rect(renderer, camera, color, rect);

}

std::string Entity::getStateString (  )
{
	return "null";
}

std::string Entity::to_json ()
{
	return "";
}

void Entity::input ( SDL_Event & event )
{

}

void Entity::draw ( SDL_Renderer * renderer, Camera * camera )
{
	if (isVisible() && currAnim)
		currAnim->draw(renderer, pos.x, pos.y);
}

int Entity::update (  )
{
	return get_state();
}
