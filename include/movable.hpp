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

#ifndef CHORA_MOVABLE_HPP
#define CHORA_MOVABLE_HPP

#include "sdl.hpp"
#include "vect.hpp"

class Movable
{
	protected:
		Vect position; // posição
		Vect velocity; // velocidade
		Vect acc; // aceleração
		SDL_Rect dim; // posição e dimensão

	public:
		Movable (  )
		{
			dim = (SDL_Rect)
			{
				0,0,1,1
			};
		}

		virtual Vect getAcceleration (  );
		virtual float getAccelerationX (  );
		virtual float getAccelerationY (  );

		virtual Vect getPosition (  );
		virtual float getPositionX (  );
		virtual float getPositionY (  );
		
		virtual Vect getVelocity (  );
		virtual float getVelocityX (  );
		virtual float getVelocityY (  );

		virtual void setAcceleration ( Vect a );
		virtual void setAccelerationX ( float x );
		virtual void setAccelerationY ( float y );

		virtual void setPosition ( Vect p );
		virtual void setPositionX ( float px );
		virtual void setPositionY ( float py );

		virtual void setVelocity ( Vect v );
		virtual void setVelocityX ( float x );
		virtual void setVelocityY ( float y );

		

		virtual void setDimension ( SDL_Rect d );
		virtual SDL_Rect getDimension (  );
};

#endif

