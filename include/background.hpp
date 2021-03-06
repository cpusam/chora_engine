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

#ifndef CHORA_BACKGROUND_HPP
#define CHORA_BACKGROUND_HPP

#include "sdl.hpp"
#include "util.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "animation.hpp"

/*
	NOTA: quando a posição é negativa não está havendo movimento do background
*/

class Background
{
	protected:
		SDL_Texture * texture;
		// se é para repetir o background no scrolling
		bool repeat;

		// posição do background no mundo
		// seria como posição inicial quando repeat = true
		Vect position;

	public:
		Background ( SDL_Texture * t=0 )
		{
			texture = t;
			repeat = true;
		}

		~Background (  )
		{
		}

		void setRepeat ( bool r )
		{
			repeat = r;
		}

		bool getRepeat (  )
		{
			return repeat;
		}

		void setPosition ( Vect p )
		{
			position = p;
		}

		Vect getPosition (  )
		{
			return position;
		}

		SDL_Texture * getTexture (  );

		bool setTexture ( SDL_Texture * t );

		/*
			scrolling nos eixos x e y mas limitado pelas bordas da surface
		*/

		void draw ( SDL_Renderer * renderer, Camera * cam, const SDL_Rect * source=nullptr, const SDL_Rect * destiny=nullptr );

		// apenas um scrolling horizontal
		void drawHorizontal ( SDL_Renderer * renderer, Camera * cam );

		// TODO: falta testar, precisa refazer
		// apenas um scrolling vertical
		void drawVertical ( SDL_Renderer * renderer, Camera * cam );
};

class AnimatedBackground: private StateMachine
{
	protected:
		Animation anim[2];

	public:
		AnimatedBackground (  )
		{
			setState(1);
		}

		~AnimatedBackground (  )
		{
			anim[0].destroyTextures();
		}

		void addFrame ( SDL_Texture *t, SDL_Rect & f, int d );

		int update ( double delta=0 ) override;

		void draw ( SDL_Renderer * renderer, Camera * cam );

		void drawHorizontal ( SDL_Renderer * renderer, Camera * cam );
};

#endif

