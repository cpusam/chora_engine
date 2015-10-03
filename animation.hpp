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

#ifndef CHORA_ANIMATION_HPP
#define CHORA_ANIMATION_HPP

#include <vector>

#include "sdl.hpp"
#include "SDL_gfx/SDL_rotozoom.hpp"
#include "statemachine.hpp"
#include "vect.hpp"
#include "camera.hpp"
#include "util.hpp"

struct STimer
{
	int state;
	float time;
	float step;

	STimer (  )
	{
		time = 0;
		step = 1;
	}

	void start (  )
	{
		state = 1;
	}
	void pause (  )
	{
		state = 0;
	}

	void reset (  )
	{
		time = 0;
	}

	int steps (  )
	{
		return int(time/step);
	}

	void update (  )
	{
		if (state)
			time += step;
	}
};

enum EAnimationState
{
	START_ANIM, // começou a animação
	CHANGE_FRAME_ANIM, // trocou de frame
	RUNNING_ANIM, // rodando a animação
	FINISHED_ANIM, // terminou a animação e vai começar a repetir os frames
	STOPED_ANIM, // terminou a animação e parou
	PAUSE_ANIM, // animação parada de trocar frames
};

/*
	CAnimationFrame contém uma cópia da surface em src quando usando SDL1.2 e um ponteiro para a
	textura quando usando SDL2.
	Para usar rotação de surfaces na animação é preciso setar para false o use_rot da animação
	antes de adicionar qualquer frame da animação. Por padrão as animações não tem rotação.
	Isso é preciso para economizar memória, apenas válido para SDL1.2.
*/
class CAnimationFrame
{
	protected:
		int delay;
		SDL_Rect source;
		float angle; // em radianos
		SVect orientation;
		SDL_Texture * texture;
		SDL_RendererFlip flip;
	public:
		int x, y; // posições relativas ao destino

	public:
		CAnimationFrame (  )
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

		CAnimationFrame ( int d, SDL_Rect s )
		{
			x = y = 0;
			set_delay(d);
			set_source(s);
			texture = 0;
			angle = 0;
			orientation.set(1,0);
		}

		void set_frame ( int d, SDL_Rect src );

		bool set_delay ( int d );
		int get_delay (  );

		void set_source ( SDL_Rect s );

		SDL_Rect get_source (  );

		SVect get_orientation (  );

		float get_angle (  );

		// rotaciona a imagem em 'a' radianos
		void rotate ( float a );
		void set_flip ( SDL_RendererFlip f );
		SDL_RendererFlip get_flip (  );
		void set_texture ( SDL_Texture * t );
		SDL_Texture * get_texture (  );

		bool destroy (  );
};

/*
	Setar use_center faz com que a posição no blit da animação use o centro do frame
	como a posição do blit.
*/
class CAnimation: public CStateMachine
{
	protected:
		STimer timer;
		int state;
		int index;
		bool use_rot; // usar rotação
		bool use_center; // usar centro do frame como posição do blit
		bool repeat;
		float angle; // em radianos
		SVect orientation;
		std::vector <CAnimationFrame> frames;

	protected:
		std::vector <SDL_Texture *> texture;
	public:
		CAnimation (  )
		{
			set_state(START_ANIM);
			repeat = true;
			index = 0;
			timer.start();
			angle = 0;
			orientation.set(1,0);
			use_rot = false;
			use_center = false;
		}

		//using CStateMachine::get_state;

		void play (  );
		void pause (  );

		void reset (  );
		void set_repeat ( bool r );

		STimer get_timer (  );

		void set_delay ( int f, int d );

		// seta todos os frames para o mesmo delay
		void set_frames_delay ( int d );

		int get_frames_size (  );

		void clear_frames ( bool destroy=false );

		SVect get_orientation (  );

		float get_angle (  );

		void rotate ( float a );

		void set_use_rot ( bool u );

		bool get_use_rot (  );

		void set_use_center ( bool u );

		bool get_use_center (  );

		void flip ( SDL_RendererFlip f );


		virtual void add_frame ( SDL_Texture * t, SDL_Rect src, int d );
		virtual void add_frame ( SDL_Texture * t, CAnimationFrame f );

		SDL_Texture * get_texture ( int i );

		void destroy_textures (  );

		bool has_texture ( SDL_Texture * t );

		bool set_index ( int i );

		int get_index (  );

		virtual CAnimationFrame get_frame ( int i );
		virtual CAnimationFrame get_curr_frame (  );

		virtual void draw ( int x, int y, SDL_Renderer * renderer );

		virtual void draw ( int x, int y, CCamera * cam, SDL_Renderer * renderer );

		virtual int update (  );
};

#endif

