#include "util.hpp"
#include <ctime>
#ifndef USE_STDRAN
	#include <random>

	std::mt19937 rng, rng_int;  // the Mersenne Twister with a popular choice of parameters
#endif

uint32_t seed_val = 0, seed_val_int = 0;

void setSeedRand(uint32_t s) {
	seed_val = s;
	#ifndef USE_STDRAN
		rng.seed(seed_val);
	#endif
}

void setSeedRandInt(uint32_t s) {
	seed_val_int = s;
	#ifndef USE_STDRAN
		rng_int.seed(seed_val_int);
	#endif
}

double Rand ( double min, double max )
{
	#if defined(USE_STDRAND)
	//removido suporte a std::random por enquanto
		static bool inited = false;
		if (!inited)
		{
			srand(time(nullptr));
			inited = true;
		}
		return max * (double(rand()) / (RAND_MAX+1.0)) + min;
	#else
		if (seed_val == 0)
		{
			setSeedRand(time(nullptr));
		}

		std::uniform_int_distribution<int> r;
		return min + (max - min) * (r(rng)/double(r.max()));
	#endif
}

int RandInt ( int min, int max )
{
	#if defined(USE_STDRAND)
		static bool inited = false;
		if (!inited)
		{
			srand(time(nullptr));
			inited = true;
		}
		return max * (double(rand()) / (RAND_MAX+1.0)) + min;
	#else
		if (seed_val_int == 0)
		{
			setSeedRandInt(time(nullptr));
		}

		std::uniform_int_distribution<int> r;
		return int(min + (max - min) * (r(rng_int)/double(r.max())));
	#endif
}


void printVideoDriver ()
{
	int numdrivers = SDL_GetNumRenderDrivers ();
	std::cout << "Render driver count: " << numdrivers << std::endl;
	for (int i=0; i<numdrivers; i++) {
	SDL_RendererInfo drinfo;
	SDL_GetRenderDriverInfo (0, &drinfo);
	std::cout << "Driver name ("<<i<<"): " << drinfo.name << std::endl;
	if (drinfo.flags & SDL_RENDERER_SOFTWARE) std::cout << " the renderer is\
	a software fallback" << std::endl;
	if (drinfo.flags & SDL_RENDERER_ACCELERATED) std::cout << " the renderer\
	uses hardware acceleration" << std::endl;
	if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC) std::cout << " present is\
	synchronized with the refresh rate" << std::endl;
	if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE) std::cout << " the\
	renderer supports rendering to texture" << std::endl;
	}
}


Uint32 getPixel ( SDL_Surface *surface, int x, int y )
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

void putPixel ( SDL_Surface *surface, int x, int y, Uint32 pixel )
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

SDL_Surface * changeColor ( SDL_Surface *surface, SDL_Color target, SDL_Color other ) {
	//SDL_Surface * tmp = SDL_CreateRGBSurface(0, surface->w, surface->h)
	return nullptr;
}

SDL_Surface * optimizeSurface ( SDL_Surface * s, SDL_Surface * screen )
{
	if (!s || !screen)
		return 0;

	return SDL_ConvertSurface(s, screen->format, 0);
}

int textureWidth ( SDL_Texture * t )
{
	if (!t)
		return 0;

	int w;
	SDL_QueryTexture(t, 0, 0, &w, 0);
	return w;
}

bool textureDimension (SDL_Texture * texture, SDL_Rect * dest) {
	int w = textureWidth(texture);
	int h = textureHeight(texture);
	
	if (w && h) {
		dest->x = 0;
		dest->y = 0;
		dest->w = w;
		dest->h = h;
		return true;
	}
	return false;
}

int textureHeight ( SDL_Texture * t )
{
	if (!t)
		return 0;

	int h;
	SDL_QueryTexture(t, 0, 0, 0, &h);
	return h;
}

void fillRect ( SDL_Renderer * renderer, Camera * cam, SDL_Color color, SDL_Rect r )
{
	SDL_Rect d;
	SDL_Rect dim;
	
	d.w = r.w;
	d.h = r.h;

	if (cam)
	{
		Vect position = cam->getPosition();
		dim = cam->getDimension();
		d.x = (dim.x + r.x) - position.x;
		d.y = (dim.y + r.y) - position.y;
		
		/*
		if (d.x < dim.x + position.x)
		{
			if (((dim.x + position.x) - d.x) < d.w)
				d.w -= ((dim.x + position.x) - d.x);
			else
				//source.w = 0; // não pode ser zero no emscripten
				return;

			d.x = dim.x;
		}
		else if (d.x + d.w > position.x + dim.x + dim.w)
		{
			if (d.x + d.w - (position.x + dim.x + dim.w) < d.w)
				d.w -= d.x + d.w - (position.x + dim.x + dim.w);
			else
				//source.w = 0; // não pode ser zero no emscripten
				return;

			d.x = d.x - position.x;
		}
		else
		{
			d.x = d.x - position.x;
		}

		if (d.y < dim.y + position.y)
		{
			if (((dim.y + position.y) - d.y) < d.h)
				d.h -= ((dim.y + position.y) - d.y);
			else
				//source.h = 0; // não pode ser zero no emscripten
				return;

			d.y = dim.y;
		}
		else if (d.y + d.h > position.y + dim.y + dim.h)
		{
			if (d.y + d.h - (position.y + dim.y + dim.h) < d.h)
				d.h -= d.y + d.h - (position.y + dim.y + dim.h);
			else
				//source.h = 0; // não pode ser zero no emscripten
				return;

			d.y = d.y - position.y;
		}
		else
		{
			d.y = d.y - position.y;
		}
		*/
	}
	else
	{
		d.x = r.x;
		d.y = r.y;
	}

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &d);
}

void fillRect ( SDL_Renderer * renderer, SDL_Color color, SDL_Rect r )
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &r);
}

int drawTexture (SDL_Renderer * renderer, SDL_Texture * texture, SDL_Rect *src, SDL_Rect *dest) {
	if (dest == nullptr && src == nullptr) {
		SDL_Rect dst = {0,0,textureWidth(texture), textureHeight(texture)};
		return SDL_RenderCopy(renderer, texture, nullptr, &dst);
	}
	else {
		return SDL_RenderCopy(renderer, texture, src, dest);
	}
}

int drawTexture ( SDL_Renderer * renderer,  Camera * cam, SDL_Texture * texture, int x, int y, int sizeW, int sizeH )
{
	if (!texture || !cam || !renderer)
	{
		return -1;
	}

	int w = 0, h = 0;
	
	SDL_QueryTexture(texture, 0, 0, &w, &h);
	SDL_Rect dest, source;

	source.w = w;
	source.h = h;

	if (source.w <= 0 || source.h <= 0)
	{
		return -2;
	}

	Vect position = cam->getPosition();
	SDL_Rect dim = cam->getDimension();

	dest.x = x + dim.x;
	dest.y = y + dim.y;

	if (dest.x < dim.x + position.x)
	{
		source.x += (dim.x + position.x) - dest.x;
		if (((dim.x + position.x) - dest.x) < source.w)
			source.w -= ((dim.x + position.x) - dest.x);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -3;

		dest.x = dim.x;
	}
	else if (dest.x + dest.w > position.x + dim.x + dim.w)
	{
		if (dest.x + dest.w - (position.x + dim.x + dim.w) < source.w)
			source.w -= dest.x + dest.w - (position.x + dim.x + dim.w);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -4;

		dest.x = dest.x - position.x;
	}
	else
	{
		dest.x = dest.x - position.x;
	}

	if (dest.y < dim.y + position.y)
	{
		source.y += (dim.y + position.y) - dest.y;
		if (((dim.y + position.y) - dest.y) < source.h)
			source.h -= ((dim.y + position.y) - dest.y);
		else
			//source.h = 0; // não pode ser zero no emscripten
			return -5;

		dest.y = dim.y;
	}
	else if (dest.y + dest.h > position.y + dim.y + dim.h)
	{
		if (dest.y + dest.h - (position.y + dim.y + dim.h) < source.h)
			source.h -= dest.y + dest.h - (position.y + dim.y + dim.h);
		else
		{
			//source.h -= dest.y + dest.h - (position.y + dim.y + dim.h);
			printf("UTIL.cpp: source.h = %d, dest... = %lf w = %d, h = %d\n", source.h,dest.y + dest.h - (position.y + dim.y + dim.h), w, h);
			//source.h = 0; // não pode ser zero no emscripten
			return -6;
		}

		dest.y = dest.y - position.y;
	}
	else
	{
		dest.y = dest.y - position.y;
	}

	if (sizeH == 0 || sizeW == 0)
	{
		dest.w = source.w;
		dest.h = source.h;
	}
	else
	{
		dest.w = sizeW;
		dest.h = sizeH;
	}

	return SDL_RenderCopy(renderer, texture, &source, &dest);
}

int drawTexture ( SDL_Renderer * renderer,  Camera * cam, SDL_Texture * texture, const SDL_Rect * src, const SDL_Rect * dst, double angle, SDL_Point * center, SDL_RendererFlip flip )
{
	if (!texture || !cam || !renderer)
	{
		return -1;
	}

	SDL_Rect dest;
	SDL_Rect source;

	if (dst)
		dest = *dst;
	else
	{
		dest.x = 0;
		dest.y = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);
	}

	if (src)
		source = *src;
	else
	{
		source.x = 0;
		source.y = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &source.w, &source.h);
	}

	if (source.w <= 0 || source.h <= 0)
	{
		return -2;
	}

	// atualiza o viewport para desenhar nele
	cam->updateViewport(renderer);

	Vect position = cam->getPosition();
	SDL_Rect dim = cam->getDimension();

	dest.x += dim.x;
	dest.y += dim.y;

	if (dest.x < dim.x + position.x)
	{
		source.x += (dim.x + position.x) - dest.x;
		if (((dim.x + position.x) - dest.x) < source.w)
			source.w -= ((dim.x + position.x) - dest.x);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -3;

		dest.x = dim.x;
	}
	else if (dest.x + dest.w > position.x + dim.x + dim.w)
	{
		if (dest.x + dest.w - (position.x + dim.x + dim.w) < source.w)
			source.w -= dest.x + dest.w - (position.x + dim.x + dim.w);
		else
			//source.w = 0; // não pode ser zero no emscripten
			return -4;

		dest.x = dest.x - position.x;
	}
	else
	{
		dest.x = dest.x - position.x;
	}

	if (dest.y < dim.y + position.y)
	{
		source.y += (dim.y + position.y) - dest.y;
		if (((dim.y + position.y) - dest.y) < source.h)
			source.h -= ((dim.y + position.y) - dest.y);
		else
			//source.h = 0; // não pode ser zero no emscripten
			return -5;

		dest.y = dim.y;
	}
	else if (dest.y + dest.h > position.y + dim.y + dim.h)
	{
		if (dest.y + dest.h - (position.y + dim.y + dim.h) < source.h)
			source.h -= dest.y + dest.h - (position.y + dim.y + dim.h);
		else
		{
			//source.h -= dest.y + dest.h - (position.y + dim.y + dim.h);
			//printf("UTIL.cpp: source.h = %d, dest... = %lf w = %d, h = %d\n", source.h,dest.y + dest.h - (position.y + dim.y + dim.h), w, h);
			//source.h = 0; // não pode ser zero no emscripten
			return -6;
		}

		dest.y = dest.y - position.y;
	}
	else
	{
		dest.y = dest.y - position.y;
	}

	return SDL_RenderCopyEx(renderer, texture, &source, &dest, angle, center, flip);
}



