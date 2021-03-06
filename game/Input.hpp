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

#ifndef CHORA_INPUT_HPP
#define CHORA_INPUT_HPP

#include "Node.hpp"

class Input: public Node
{
	public:
		enum State: int
		{
			FREE,
			PRESS,
			HOLD,
			RELEASE,
		};

	public:
		Input();
		Input ( int k );
		virtual ~Input();
		
		Uint32 getPressTime (  );

		//retorna se está pressionado ou não
		bool isPressed();
		//define se está pressionado ou não
		virtual void setPressed ( bool p );
		virtual void setInput ( int k );
		virtual int getInput (  );
		
		virtual void reset (  );
		void input ( SDL_Event & event ) override;
		int update ( double delta=0 ) override;
	
	protected:
		int in;
		bool press;
		//tempo que foi pressionado
		Uint32 pressTime;
};

#endif
