#include "background.hpp"

SDL_Texture * Background::getTexture (  )
{
	return texture;
}

bool Background::setTexture ( SDL_Texture * t )
{
	if (texture && texture != t)
		SDL_DestroyTexture(texture);

	texture = t;

	return (texture != 0);
}

/*
	scrolling nos eixos x e y mas limitado pelas bordas da surface
*/

void Background::draw ( SDL_Renderer * renderer, Camera * cam, const SDL_Rect * source, const SDL_Rect * destiny )
{
	Vect p;
	SDL_Rect d, src;
	int w, h;

	if (!texture)
		return;

	if (!destiny)
		SDL_QueryTexture(texture, 0, 0, &w, &h);

	bool withDestiny = false;
	if (cam && !destiny)
	{
		p = cam->getPosition() + position;
		d = cam->getDimension();
		src = d;
	}
	else if (!destiny)
	{
		int rw, rh;
		SDL_RenderGetLogicalSize(renderer, &rw, &rh);
		d = (SDL_Rect){0,0,rw,rh};
		src = d;
	}
	else {
		withDestiny = true;
	}

	src.x = int(p.x);
	src.y = int(p.y);

	if (p.x < 0)
	{
		src.x = 0;
	}
	else if (p.x + d.w > w)
	{
		src.x = w - d.w;
	}

	if (p.y < 0)
	{
		src.y = 0;
	}
	else if (p.y + d.h > h)
	{
		src.y = h - d.h;
	}

	if (!withDestiny)
		SDL_RenderCopy(renderer, texture, &src, &d);
	else
		SDL_RenderCopy(renderer, texture, source, destiny);
}
// apenas um scrolling horizontal
void Background::drawHorizontal ( SDL_Renderer * renderer, Camera * cam )
{
	if (!texture)
		return;
	
	Vect p;
	int w, h;
	SDL_Rect dim = cam->getDimension(), src, dest;
	SDL_QueryTexture(texture, 0, 0, &w, &h);

	Vect pCam = cam->getPosition();

	float ratioW = float(static_cast<int>(roundf(pCam.x + position.x)) % dim.w) / float(dim.w);
	//src a direita
	if (w == dim.w)
	{
		src.x = static_cast<int>(SDL_fabs(pCam.x + position.x)) % w;
		src.w = w - src.x;
	}
	else
	{
		src.x = int(w * ratioW);
		src.w = w - int(w * ratioW);
	}
	src.y = 0;
	src.h = h;

	//destino a esquerda
	dest.x = dim.x;
	dest.y = 0;
	dest.w = dim.w - static_cast<int>(roundf(pCam.x + position.x)) % dim.w;
	dest.h = dim.h;
	SDL_RenderCopy(renderer, texture, &src, &dest);
	if (dest.w < dim.w)
	{
		//src a direita
		src.x = 0;
		if (w == dim.w)
			src.w = static_cast<int>(pCam.x + position.x) % w;
		else
			src.w = w * ratioW;
		
		//destino a direita
		dest.x = dest.w;
		dest.y = 0;
		dest.w = dim.w - dest.w;
		dest.h = dim.h;
		SDL_RenderCopy(renderer, texture, &src, &dest);
	}
}

// NOTE: falta testar, precisa refazer
// apenas um scrolling vertical
void Background::drawVertical ( SDL_Renderer * renderer, Camera * cam )
{
	
	if (!texture)
		return;
	
	Vect p;
	int w, h;
	SDL_Rect dim = cam->getDimension(), src, dest;
	SDL_QueryTexture(texture, 0, 0, &w, &h);

	Vect pCam = cam->getPosition();

	float ratioH = float(static_cast<int>(roundf(pCam.y + position.y)) % h) / float(dim.h);
	if (h == dim.h)
	{
		src.y = static_cast<int>(SDL_fabs(pCam.y + position.y)) % h;
		src.h = h - src.y;
	}
	else
	{
		src.y = h * ratioH;
		src.h = h - src.y;
	}
	src.x = 0;
	src.w = w;
	dest.x = 0;
	dest.y = 0;
	dest.h = dim.h - static_cast<int>(roundf(pCam.y + position.y)) % dim.h;
	dest.w = dim.w;
	SDL_RenderCopy(renderer, texture, &src, &dest);
	if (dest.h < dim.h)
	{
		src.y = 0;
		if (h == dim.h)
			src.h = static_cast<int>(pCam.y) % h;
		else
			src.h = h * ratioH;
		dest.y = dest.h;
		dest.x = 0;
		dest.h = dim.h - dest.h;
		dest.w = dim.w;
		SDL_RenderCopy(renderer, texture, &src, &dest);
	}
}

/////////////////////////////////////////////////////////////

void AnimatedBackground::addFrame ( SDL_Texture *t, SDL_Rect & f, int d )
{
	anim[0].addFrame(t, f, d);
	anim[1].addFrame(t, f, d);
}


int AnimatedBackground::update ( double delta )
{
	int ret = DEFAULT_STATE;
	switch (getState())
	{
	case 0:
		break;
	case 1:
		anim[0].update();
		ret = anim[1].update();
		break;
	}

	return ret;
}

void AnimatedBackground::draw ( SDL_Renderer * renderer, Camera * cam )
{
	Vect p;
	SDL_Rect d, src;
	int w, h;

	if (!anim[0].getTexture(0))
		return;

	SDL_QueryTexture(anim[0].getTexture(0), 0, 0, &w, &h);

	p = cam->getPosition();
	d = cam->getDimension();
	src = d;

	src.x = int(p.x);
	src.y = int(p.y);

	if (p.x < 0)
	{
		src.x = 0;
	}
	else if (p.x + d.w > w)
	{
		src.x = w - d.w;
	}

	if (p.y < 0)
	{
		src.y = 0;
	}
	else if (p.y + d.h > h)
	{
		src.y = h - d.h;
	}

	SDL_RenderCopy(renderer, anim[0].getTexture(0), &src, &d);
}

void AnimatedBackground::drawHorizontal ( SDL_Renderer * renderer, Camera * cam )
{
	Vect p;
	SDL_Rect d, dim, src, surf;

	if (!anim[0].getTexture(0))
		return;

	p = cam->getPosition();
	d = dim = cam->getDimension();
	surf = src = anim[0].getCurrentFrame().getSourceRect();

	src.y = surf.y + int(p.y) % surf.h;
	if (p.y < 0)
		src.y = surf.y;
	else if (p.y + dim.h >= surf.h)
		src.y = surf.y + (surf.h - dim.h);

	src.x = surf.x + int(p.x) % surf.w;
	src.w = dim.w;
	src.h = dim.h;
	d.x = dim.x;
	SDL_RenderCopy(renderer, anim[0].getTexture(0), &src, &d);

	if (int(p.x) % surf.w > surf.w - dim.w)
	{
		src.x = surf.x;
		src.w = int(p.x) % surf.w - (surf.w - dim.w);
		d.x = dim.x + surf.w - int(p.x) % surf.w;
		SDL_RenderCopy(renderer, anim[0].getTexture(0), &src, &d);
	}
}


