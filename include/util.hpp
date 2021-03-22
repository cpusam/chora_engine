/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

/*
	funções de uso geral
*/
#ifndef CHORA_UTIL_HPP
#define CHORA_UTIL_HPP

#include "sdl.hpp"
#include "camera.hpp"

extern double Rand ( double min=0.0, double max=1.0 );
extern int RandInt ( int min, int max );

extern void printVideoDriver (  );

extern Uint32 getPixel ( SDL_Surface * surface, int x, int y );
extern void putPixel ( SDL_Surface *surface, int x, int y, Uint32 pixel );

extern SDL_Surface * changeColor ( SDL_Surface *surface, SDL_Color target, SDL_Color other );

extern SDL_Surface * optimizeSurface ( SDL_Surface * s, SDL_Surface * screen );

extern int textureWidth ( SDL_Texture * t );

extern int textureHeight ( SDL_Texture * t );

extern void fillRect ( SDL_Renderer * renderer, Camera * cam, SDL_Color color, SDL_Rect r );

extern void fillRect ( SDL_Renderer * renderer, SDL_Color color, SDL_Rect r );

extern int drawTexture (SDL_Renderer * renderer, SDL_Texture * texture, SDL_Rect *src, SDL_Rect *dest);

extern int drawTexture ( SDL_Renderer * renderer,  Camera * cam, SDL_Texture * texture, int x, int y, int sizeW=0, int sizeH=0 );

extern int drawTexture ( SDL_Renderer * renderer,  Camera * cam, SDL_Texture * texture, const SDL_Rect * src, const SDL_Rect * dst, double angle, SDL_Point * center, SDL_RendererFlip flip );

#endif

