#include "util.hpp"

Uint32 get_pixel ( SDL_Surface *surface, int x, int y )
{
	/*
	Uint32 * p = (Uint32 *)surface->pixels;

	return p[(y * surface->w) + x];
	*/
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to retrieve
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			return 0;	   // shouldn't happen, but avoids warnings
	}
	
	return 0;
}

void put_pixel ( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	/*
	Uint32 * p = (Uint32 *)surface->pixels;

	p[(y * surface->w) + x] = pixel;
	*/
	int bpp = surface->format->BytesPerPixel;
	// Here p is the address to the pixel we want to set
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

		switch(bpp) {
			case 1:
				*p = pixel;
				break;

			case 2:
				*(Uint16 *)p = pixel;
				break;

			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;

			case 4:
				*(Uint32 *)p = pixel;
				break;

			default:
				break;
	}
}

SDL_Surface * optimize_surface ( SDL_Surface * s, SDL_Surface * screen )
{
	if (!s || !screen)
		return 0;

	return SDL_ConvertSurface(s, screen->format, 0);
}

int texture_width ( SDL_Texture * t )
{
	int w;
	if (!t)
		return 0;

	SDL_QueryTexture(t, NULL, NULL, &w, NULL);
	return w;
}

int texture_height ( SDL_Texture * t )
{
	int h;
	if (!t)
		return 0;

	SDL_QueryTexture(t, NULL, NULL, NULL, &h);
	return h;
}

void fill_rect ( CCamera * cam, SDL_Renderer * renderer, SDL_Color color, SDL_Rect r )
{
	SDL_Rect d;
	SDL_Rect dim;
	
	d.w = r.w;
	d.h = r.h;

	if (cam)
	{
		dim = cam->get_dimension();
		d.x = r.x + dim.x;
		if (d.x < dim.x)
		{
			d.w = r.w - (dim.x - d.x);
			if (d.w <= 0)
				return;
			d.x = dim.x;
		}
		else if (d.x + d.w > dim.x + dim.w)
		{
			d.w = r.w - ((d.x + d.w) - (dim.x + dim.w));
			if (d.w <= 0)
				return;
			d.x = dim.x + dim.w - d.w;
		}
		else
		{
			d.w = r.w;
		}

		d.y = r.y + dim.y;
		if (d.y < dim.y)
		{
			d.h = r.h - (dim.y - d.y);
			if (d.h <= 0)
				return;
			d.y = dim.y;
		}
		else if (d.y + d.h > dim.y + dim.h)
		{
			d.h = r.h - ((d.y + d.h) - (dim.y + dim.h));
			if (d.h <= 0)
				return;
			d.y = dim.y + dim.h - d.h;
		}
		else
		{
			d.h = r.h;
		}
	}
	else
	{
		d.x = r.x;
		d.y = r.y;
	}

	

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &d);
}

int draw_texture ( SDL_Texture * texture, int x, int y, CCamera * cam, SDL_Renderer * renderer )
{
	if (!texture || !cam || !renderer)
	{
		return -1;
	}

	int w = 0, h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect dest, source;

	source.w = w;
	source.h = h;

	if (source.w <= 0 || source.h <= 0)
	{
		return -2;
	}

	SVect pos = cam->get_position();
	SDL_Rect dim = cam->get_dimension();

	dest.x = x + dim.x;
	dest.y = y + dim.y;

	if (dest.x < dim.x + pos.x)
	{
		source.x += (dim.x + pos.x) - dest.x;
		if (((dim.x + pos.x) - dest.x) < source.w)
			source.w -= ((dim.x + pos.x) - dest.x);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -3;

		dest.x = dim.x;
	}
	else if (dest.x + dest.w > pos.x + dim.x + dim.w)
	{
		if (dest.x + dest.w - (pos.x + dim.x + dim.w) < source.w)
			source.w -= dest.x + dest.w - (pos.x + dim.x + dim.w);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -4;

		dest.x = dest.x - pos.x;
	}
	else
	{
		dest.x = dest.x - pos.x;
	}

	if (dest.y < dim.y + pos.y)
	{
		source.y += (dim.y + pos.y) - dest.y;
		if (((dim.y + pos.y) - dest.y) < source.h)
			source.h -= ((dim.y + pos.y) - dest.y);
		else
			//source.h = 0; // não pode ser zero no emscripten
			return -5;

		dest.y = dim.y;
	}
	else if (dest.y + dest.h > pos.y + dim.y + dim.h)
	{
		if (dest.y + dest.h - (pos.y + dim.y + dim.h) < source.h)
			source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
		else
		{
			source.h -= dest.y + dest.h - (pos.y + dim.y + dim.h);
			printf("source.h = %d, dest... = %lf w = %d, h = %d\n", source.h,dest.y + dest.h - (pos.y + dim.y + dim.h), w, h);
			//source.h = 0; // não pode ser zero no emscripten
			return -6;
		}

		dest.y = dest.y - pos.y;
	}
	else
	{
		dest.y = dest.y - pos.y;
	}

	dest.w = source.w;
	dest.h = source.h;

	return SDL_RenderCopy(renderer, texture, &source, &dest);
}


