/*
 Copyright (C) 2014-2018 Samuel Leonardo

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

#ifndef KEY_HPP
#define KEY_HPP

#include "../sdl.hpp"
#include "../statemachine.hpp"

class Key: public StateMachine
{
	public:
		enum KeyState: int
		{
			FREE,
			PRESS,
			HOLD,
			RELEASE,
		};

	public:
		Key();
		Key ( SDL_Keycode k );
		virtual ~Key();
		
		Uint32 getPressTime (  );

		bool isPressed();
		void setKey ( SDL_Keycode k );
		SDL_Keycode getKey (  );
		
		virtual void reset (  );
		virtual void input ( SDL_Event & event );
		virtual int update (  );
	
	protected:
		SDL_Keycode key;
		bool press;
		
	private:
		Uint32 pressTime;
};

#endif // KEY_HPP
