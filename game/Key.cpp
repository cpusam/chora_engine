#include "Key.hpp"

Key::Key()
{
	key = 0;
	press = false;
	set_state(FREE);
}

Key::Key ( SDL_Keycode k )
{
	pressTime = 0;
	key = k;
	press = false;
	set_state(FREE);
}

Key::~Key()
{
	//dtor
}

Uint32 Key::getPressTime (  )
{
	return pressTime;
}

void Key::setKey ( SDL_Keycode k )
{
	key = k;
}

SDL_Keycode Key::getKey (  )
{
	return key;
}

void Key::reset (  )
{
	press = false;
	set_state(FREE);
}

void Key::input ( SDL_Event & event )
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == key)
		{
			press = true;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == key)
			press = false;
	}
}

int Key::update (  )
{
	switch (get_state())
	{
		case FREE:
			if (press)
			{
				pressTime = SDL_GetTicks();
				set_state(PRESS);
			}
			break;
		
		case PRESS:
			if (press)
			{
				set_state(HOLD);
			}
			else
			{
				set_state(RELEASE);
			}
			break;
		
		case HOLD:
			if (press == false)
				set_state(RELEASE);
			break;
		
		case RELEASE:
			press = false;
			set_state(FREE);
			break;
		
		default:
			break;
	}
	
	return get_state();
}

