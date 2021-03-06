#include "label.hpp"
#include "writer.hpp"

GuiLabel::GuiLabel ( std::string s, SDL_Color c, const std::string fontName )
{
	str = "";
	color = c;
	texture = nullptr;
	/*
		if (s != "")
			setString(s);
		else
			str = "";
		*/
	if (s == "")
		s = " ";
	str_to_surface(s,fontName);
	this->fontName = fontName;
	if (s != "" && !texture)
			throw Exception("GuiLabel::erro ao setar texture");
	
	printf("Texture ptr = %p\n", texture);
}

GuiLabel::~GuiLabel (  )
{
	if (texture)
		SDL_DestroyTexture(texture);
	texture = nullptr;
}

void GuiLabel::str_to_surface ( std::string s, std::string fontName )
{
	int w, h;
	if (texture)
		SDL_DestroyTexture(texture);

	texture = nullptr;
	try {
		texture = Writer::instance()->renderText(fontName, s, color, SOLID_TEXT);
	} catch(Exception & e){
		e.what();
		abort();
	}

	if (!texture)
	{
		char * e = new char[256];
		sprintf(e, "GuiLabel: erro %s\n", SDL_GetError());
		throw Exception(e);
	}

	SDL_QueryTexture(texture, 0, 0, &w, &h);

	str = s;

	dim.x = position.x + rel_pos.x, dim.y = position.y + rel_pos.y;
	dim.w = w, dim.h = h;
}

void GuiLabel::setColor ( SDL_Color c )
{
	color = c;
	str_to_surface(str);
}

void GuiLabel::setString ( std::string s, std::string fontName )
{
	/*
	if (s == str && texture)
		return;
	*/

	str = s;

	str_to_surface(s, fontName);
}

SDL_Texture * GuiLabel::getTexture (  )
{
	return texture;
}

void GuiLabel::setTexture ( SDL_Texture * t )
{
	if (texture && texture != t)
		SDL_DestroyTexture(texture);

	texture = t;

	if (texture)
	{
		int w, h;
		SDL_QueryTexture(texture, 0, 0, &w, &h);
		dim.w = w;
		dim.h = h;
	}
	else
	{
		dim.w = dim.h = 0;
	}
}

int GuiLabel::getTextureWidth (  )
{
	int w;

	if (!texture)
		return 0;

	SDL_QueryTexture(texture, 0, 0, &w, 0);
	return w;
}

int GuiLabel::getTextureHeight (  )
{
	int h;

	if (!texture)
		return 0;

	SDL_QueryTexture(texture, 0, 0, 0, &h);
	return h;
}

std::string GuiLabel::getString (  )
{
	return str;
}

void GuiLabel::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;

	childDraw(renderer);
	SDL_Rect d = {int(position.x),int(position.y),dim.w,dim.h};
	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &d);
}

///////////////////////////////////////////////////////////

void GuiLabelNumber::proc_value ( float v )   // para ser usada internamente
{
	int n = v, a, i;
	std::string s, aux;
	value = v;

	for (i = 0; n > 0; i++)
	{
		a = n % 10;
		n = (n - a) / 10;
		aux.push_back('0' + a);
	}

	// inverte aux;
	for (i = aux.size() - 1; i > -1; i--)
		s.push_back(aux[i]);

	aux.swap(s);
	s.clear();

	if (aux.size() < left_zero)
	{
		for (i = 0; i < left_zero - aux.size(); i++)
			s.push_back('0');

		s.append(aux);
	}
	else
	{
		s.swap(aux);
	}

	setString(s);
}

void GuiLabelNumber::set_left_zero ( int lz )
{
	if (lz > -1 && lz != left_zero)
	{
		left_zero = lz;
		proc_value(value);
	}
}

bool GuiLabelNumber::set_value ( float v )
{
	if (value == v)
		return false;

	proc_value(int(v));
	value = v;

	return true;
}

