#include "background.hpp"

SDL_Texture * Background::get_texture (  )
{
	return texture;
}

bool Background::set_texture ( SDL_Texture * t )
{
	if (texture && texture != t)
		SDL_DestroyTexture(texture);

	texture = t;

	return (texture != 0);
}

/*
	scrolling nos eixos x e y mas limitado pelas bordas da surface
*/

void Background::draw ( Camera * cam, SDL_Renderer * renderer )
{
	Vect p;
	SDL_Rect d, src;
	int w, h;

	if (!texture)
		return;

	SDL_QueryTexture(texture, 0, 0, &w, &h);

	if (cam)
	{
		p = cam->get_position() + pos;
		d = cam->get_dimension();
		src = d;
	}
	else
	{
		int rw, rh;
		SDL_RenderGetLogicalSize(renderer, &rw, &rh);
		d = (SDL_Rect)
		{
			0,0,rw,rh
		};
		src = d;
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

	SDL_RenderCopy(renderer, texture, &src, &d);
}
// apenas um scrolling horizontal
void Background::draw_hor ( Camera * cam, SDL_Renderer * renderer )
{
	Vect p;
	SDL_Rect d, dim, src;
	int w, h;

	if (!texture)
		return;

	SDL_QueryTexture(texture, 0, 0, &w, &h);

	p = cam->get_position() + pos;
	d = dim = cam->get_dimension();
	src = d;

	if (repeat == false)
	{
		/*
		#ifndef USE_SDL2
			draw_surface(surface, pos.x, pos.y, cam, screen);
		#else
			draw_texture(texture, pos.x, pos.y, cam, renderer);
		#endif

		return;
		*/

		Vect cp = cam->get_position();

		if (cp.x < pos.x)
		{
			src.x = 0;
			if (dim.w - int(pos.x - cp.x) > 0)
				src.w = dim.w - int(pos.x - cp.x);
			else
				return;

			d.x = dim.x + (pos.x - cp.x);
			d.w = src.w;
		}
		else if (cp.x + dim.w > pos.x + w)
		{
			src.x = w - (int(pos.x + w) - int(cp.x));

			if (w - src.x > 0)
				src.w = w - src.x;
			else
				return;

			d.x = dim.x;
			d.w = src.w;
		}
		else
		{
			src.x = int(cp.x - pos.x);
			src.w = dim.w;

			if (src.w > w)
				src.w = w;
			else if (src.w == 0)
				return;

			d.x = dim.x;
			d.w = src.w;
		}

		if (cp.y < pos.y)
		{
			src.y = 0;
			if (dim.h - int(pos.y - cp.y) > 0)
				src.h = dim.h - int(pos.y - cp.y);
			else
				return;

			d.y = dim.y + (pos.y - cp.y);
			d.h = src.h;
		}
		else if (cp.y + dim.h > pos.y + h)
		{
			src.y = h - (int(pos.y + h) - int(cp.y));

			if (h - src.y > 0)
				src.h = h - src.y;
			else
				return;

			d.y = dim.y;
			d.h = src.h;
		}
		else
		{
			src.y = int(cp.y - pos.y);
			src.h = dim.h;

			if (src.h > h)
				src.h = h;
			else if (src.h == 0)
				return;

			d.y = dim.y;
			d.h = src.h;
		}

		SDL_RenderCopy(renderer, texture, &src, &d);

		return;
	}

	src.y = int(p.y);
	if (p.y < 0)
		src.y = 0;
	else if (p.y + dim.h > h)
		src.y = h - dim.h;



	if (p.x < 0)
	{
		if (int(p.x) % w < -dim.w)
		{
			src.x = w + int(p.x) % w;
			src.w = dim.w;
			d.x = dim.x;
		}
		else
		{
			src.x = 0;
			src.w = dim.w + int(p.x) % w;
			d.x = dim.x - int(p.x) % w;
		}
		SDL_RenderCopy(renderer, texture, &src, &d);

		src.x = w + int(p.x) % w;
		src.w = dim.w;
		d.x = dim.x;
		SDL_RenderCopy(renderer, texture, &src, &d);
	}
	else
	{
		// para o destino a esquerda
		src.x = int(p.x) % w;
		src.w = w - int(p.x) % w;
		d.x = dim.x;
		d.w = src.w;
		SDL_RenderCopy(renderer, texture, &src, &d);

		if (int(p.x) % w > w - dim.w)
		{
			src.x = 0;
			src.w = int(p.x) % w - (w - dim.w);
			d.x = dim.x + w - int(p.x) % w;
			d.w = src.w;
			SDL_RenderCopy(renderer, texture, &src, &d);
		}
	}
}

// NOTE: falta testar, precisa refazer
// apenas um scrolling vertical
void Background::draw_ver ( Camera * cam, SDL_Renderer * renderer )
{
	Vect p;
	SDL_Rect d, dim, src;
	int w, h;

	if (!texture)
		return;

	SDL_QueryTexture(texture, 0, 0, &w, &h);

	p = cam->get_position() + pos;
	d = dim = cam->get_dimension();
	src = d;

	if (p.y < 0)
	{
		src.x = int(p.x);
		if (p.x < 0)
			src.x = 0;
		else if (p.x + dim.w > w)
			src.w = w - dim.w;

		if (int(p.y) % h < -dim.h)
		{
			src.y = h + int(p.y) % h;
			src.h = dim.h;
			d.y = dim.y;
		}
		else
		{
			src.y = 0;
			src.h = dim.h + int(p.y) % h;
			d.y = dim.y - int(p.x) % h;
		}
		SDL_RenderCopy(renderer, texture, &src, &d);

		src.y = h + int(p.y) % h;
		src.h = dim.h;
		d.y = dim.y;
		SDL_RenderCopy(renderer, texture, &src, &d);
	}
	else
	{
		src.x = int(p.x);
		if (p.x < 0)
			src.x = 0;
		else if (p.x + dim.w > w)
			src.x = w - dim.w;

		src.y = int(p.y) % h;
		src.h = dim.h;
		d.y = dim.y;
		SDL_RenderCopy(renderer, texture, &src, &d);

		if (int(p.y) % h > h - dim.h)
		{
			src.y = 0;
			src.h = int(p.y) % h - (h - dim.h);
			d.y = dim.y + h - int(p.y) % h;
			SDL_RenderCopy(renderer, texture, &src, &d);
		}
	}
}

/////////////////////////////////////////////////////////////

void AnimatedBackground::add_frame ( SDL_Texture *t, SDL_Rect f, int d )
{
	anim[0].add_frame(t, f, d);
	anim[1].add_frame(t, f, d);
}


int AnimatedBackground::update (  )
{
	int ret = DEFAULT_STATE;
	switch (get_state())
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

void AnimatedBackground::draw ( Camera * cam, SDL_Renderer * renderer )
{
	Vect p;
	SDL_Rect d, src;
	int w, h;

	if (!anim[0].get_texture(0))
		return;

	SDL_QueryTexture(anim[0].get_texture(0), 0, 0, &w, &h);

	p = cam->get_position();
	d = cam->get_dimension();
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

	SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);
}

void AnimatedBackground::draw_hor ( Camera * cam, SDL_Renderer * renderer )
{
	Vect p;
	SDL_Rect d, dim, src, surf;

	if (!anim[0].get_texture(0))
		return;

	p = cam->get_position();
	d = dim = cam->get_dimension();
	surf = src = anim[0].get_curr_frame().get_source();

	src.y = surf.y + int(p.y) % surf.h;
	if (p.y < 0)
		src.y = surf.y;
	else if (p.y + dim.h >= surf.h)
		src.y = surf.y + (surf.h - dim.h);

	src.x = surf.x + int(p.x) % surf.w;
	src.w = dim.w;
	src.h = dim.h;
	d.x = dim.x;
	SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);

	if (int(p.x) % surf.w > surf.w - dim.w)
	{
		src.x = surf.x;
		src.w = int(p.x) % surf.w - (surf.w - dim.w);
		d.x = dim.x + surf.w - int(p.x) % surf.w;
		SDL_RenderCopy(renderer, anim[0].get_texture(0), &src, &d);
	}
}


