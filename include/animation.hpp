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

#ifndef CHORAATION_HPP
#define CHORAATION_HPP

#include <vector>

#include "sdl.hpp"
//#include "SDL_gfx/SDL_rotozoom.hpp"
#include "SDL_gfx/SDL_framerate.hpp"
#include "statemachine.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "util.hpp"


struct STimer
{
	int state;
	float ticks;
	float step;
	Uint32 lastTick;

	public:
		STimer (  )
		{
			state = 1;
			ticks = 0;
			step = 1;
			lastTick = SDL_GetTicks();
		}

		void start (  )
		{
			state = 1;
			lastTick = SDL_GetTicks();
		}
		void pause (  )
		{
			state = 0;
		}

		void reset (  )
		{
			ticks = 0;
			lastTick = SDL_GetTicks();
		}

		int steps (  )
		{
			return int(ticks);
		}

		void update (  )
		{
			if (state)
			{
				Uint32 tick = SDL_GetTicks();
				//#warning "Aqui deveria usar FPSManager::getDelta para tempo gasto"
				step = FPSManager::instance()->getDelta();//(float)tick - lastTick;
				
				ticks += step;
				lastTick = tick;
				
			}
		}
};


/*
	AnimationFrame contém uma cópia da surface em src quando usando SDL1.2 e um ponteiro para a
	textura quando usando SDL2.
	Para usar rotação de surfaces na animação é preciso setar para false o use_rot da animação
	antes de adicionar qualquer frame da animação. Por padrão as animações não tem rotação.
	Isso é preciso para economizar memória, apenas válido para SDL1.2.
*/
class AnimationFrame
{
	protected:
		int delay;
		SDL_Rect source, destiny;
		float angle; // em radianos
		Vect orientation;
		SDL_Texture * texture;
		SDL_RendererFlip flip;
	public:
		int x, y; // posições relativas ao destino

	public:
		AnimationFrame (  )
		{
			x = y = 0;
			delay = 0;
			source = (SDL_Rect)
			{
				0,0,0,0
			};
			texture = 0;
			flip = SDL_FLIP_NONE;
			angle = 0;
			orientation.set(1,0);
		}

		AnimationFrame ( int d, SDL_Rect s )
		{
			x = y = 0;
			setDelay(d);
			setSourceRect(s);
			texture = 0;
			angle = 0;
			orientation.set(1,0);
			flip = SDL_FLIP_NONE;
		}
		
		AnimationFrame ( SDL_Texture * t, int d, SDL_Rect s, SDL_Rect dst )
		{
			x = dst.x;
			y = dst.y;
			setDelay(d);
			setSourceRect(s);
			setDestinyRect(dst);
			texture = t;
			angle = 0;
			orientation.set(1,0);
			flip = SDL_FLIP_NONE;
		}
		
		AnimationFrame ( const AnimationFrame & frame )
		{
			x = frame.destiny.x;
			y = frame.destiny.y;
			setDelay(frame.delay);
			setSourceRect(frame.source);
			setDestinyRect(frame.destiny);
			texture = frame.texture;
			angle = frame.angle;
			orientation = frame.orientation;
			flip = frame.flip;
		}

		void setFrame ( int d, SDL_Rect src );

		bool setDelay ( int d );
		int getDelay (  );

		void setSourceRect ( const SDL_Rect & s );

		SDL_Rect getSourceRect (  );

		void setDestinyRect ( const SDL_Rect & d );

		SDL_Rect getDestinyRect (  );

		Vect getOrientation (  );

		float getAngle (  );
		void setAngle ( float rad );

		// rotaciona a imagem em 'a' radianos
		void rotate ( float a );
		void setFlip ( bool hor, bool ver );
		SDL_RendererFlip get_flip (  );
		void setTexture ( SDL_Texture * t );
		SDL_Texture * getTexture (  );

		bool destroy (  );
};

/*
	Setar use_center faz com que a posição no blit da animação use o centro do frame
	como a posição do blit.
*/
class Animation: public StateMachine
{
	protected:
		STimer timer;
		int state;
		int index;
		bool use_rot; // usar rotação
		bool use_center; // usar centro do frame como posição do blit
		bool repeat;
		float angle; // em radianos
		Vect orientation, center;
		std::vector <AnimationFrame> frames;
	
	public:
		enum EAnimationState
		{
			START, // começou a animação
			CHANGE_FRAME, // trocou de frame
			RUNNING, // rodando a animação
			FINISHED, // terminou a animação e vai começar a repetir os frames
			STOPPED, // terminou a animação e parou
			PAUSED, // animação parada de trocar frames
		};

	protected:
		std::vector <SDL_Texture *> texture;
		std::string name;
	public:
		Animation (  )
		{
			setState(START);
			repeat = true;
			index = 0;
			timer.start();
			angle = 0;
			orientation.set(1,0);
			use_rot = false;
			use_center = false;
			name = "";
		}

		//using StateMachine::getState;

		void play (  );
		void pause (  );

		void reset (  );
		void setRepeat ( bool r );

		void setFrameDelay ( int f, int d );
		int getTotalTime (  );

		// seta todos os frames para o mesmo delay
		void setFramesDelay ( int d );

		int getFramesSize (  );

		void clearFrames ( bool destroy=false );

		Vect getOrientation (  );

		float getAngle (  );
		void setAngle ( float rad );

		void rotate ( float a );

		void setUseRotation ( bool u );

		bool getUseRotation (  );

		void setUseCenter ( bool u );
		bool getUseCenter (  );

		void setCenter ( Vect center );
		
		SDL_RendererFlip get_flip (  );
		
		void flip ( SDL_RendererFlip f );
		void flip ( bool hor, bool ver );

		virtual void addFrame ( SDL_Texture * t, AnimationFrame & f );
		virtual void addFrame ( SDL_Texture * t, SDL_Rect const & src, int d );
		// remover esse destiny e adicionar um w,h no draw como na libgdx
		virtual void addFrame ( SDL_Texture * t, SDL_Rect const & src, SDL_Rect const & dst, int d );

		SDL_Texture * getTexture ( int i );

		void destroyTextures (  );

		bool hasTexture ( SDL_Texture * t );

		bool setIndex ( int i );

		int getIndex (  );
		
		void setTimer ( STimer t );
		STimer getTimer (  );

		void setName ( std::string n );
		std::string getName (  );

		std::vector<AnimationFrame> & getFrames (  );
		virtual AnimationFrame getFrame ( int i );
		virtual AnimationFrame getCurrentFrame (  );

		virtual int draw ( SDL_Renderer * renderer, int x, int y );

		virtual int draw ( SDL_Renderer * renderer, Camera * cam, int x, int y );
		
		virtual int draw ( SDL_Renderer * renderer, Camera * cam, int x, int y, int destW, int destH );

		virtual int update (  );
};

#endif

