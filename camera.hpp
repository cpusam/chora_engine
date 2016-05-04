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

#ifndef CHORA_CAMERA_HPP
#define CHORA_CAMERA_HPP

#include <iostream>

#include "sdl.hpp"
#include "vect.hpp"

class Camera
{
	protected:
		Vect position; // posição dentro do jogo
		Vect focus;
		SDL_Rect dimension;
		SDL_Rect limit;
		SDL_Rect action_area;

	public:
		Camera ( SDL_Rect d, SDL_Rect l )
		{
			dimension = d;
			limit = l;
			// focus padrão no meio da camera
			focus.x = d.w / 2.0f;
			focus.y = d.h / 2.0f;
			//lookat(Vect(0,0));
			if (limit.w < dimension.w || limit.h < dimension.h)
				std::cout << "Camera: atenção limit W ou H menor que dimension W ou H\n";
			
			action_area = (SDL_Rect){0,0,d.w,d.h};
		}

		void lookat ( Vect p );
		Vect get_position (  );
		Vect get_focus (  );
		SDL_Rect get_view (  ); // retorna posição da camera junto com suas dimensões
		void set_position ( Vect p );
		int set_focus ( Vect f );
		SDL_Rect get_dimension (  );
		SDL_Rect get_limit (  );
		void setScreenPos ( int x, int y );
		void set_limit ( SDL_Rect l );
		
		// se está dentro da área de ação
		bool inAction ( Vect p );
		bool inAction ( SDL_Rect d );
		
		void setActionArea ( SDL_Rect d );
		SDL_Rect getActionArea (  );
		
};

#endif

