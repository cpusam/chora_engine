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

#ifndef CHORA_STATEMACHINE_HPP
#define CHORA_STATEMACHINE_HPP

#include <vector>
#include "sdl.hpp"

#define DEFAULT_STATE 0

class StateMachine
{
	private:
		int state;

	public:
		StateMachine (  )
		{
			state = DEFAULT_STATE;
		}

		virtual ~StateMachine (  )
		{
		}

		virtual void setState ( int s );
		virtual int getState (  );
		virtual void input ( SDL_Event & event );
		virtual int update ( double delta=0 );
};

#endif


