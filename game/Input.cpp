#include "Input.hpp"

Input::Input()
{
	in = -1;
	press = false;
	setState(State::FREE);
}

Input::Input ( int k )
{
	pressTime = 0;
	in = k;
	press = false;
	setState(State::FREE);
}

Input::~Input()
{
	//dtor
}

Uint32 Input::getPressTime (  )
{
	return pressTime;
}

bool Input::isPressed()
{
	return press;
}

void Input::setInput ( int k )
{
	in = k;
}

int Input::getInput (  )
{
	return in;
}

void Input::reset (  )
{
	press = false;
	setState(State::FREE);
}

void Input::input ( SDL_Event & event )
{
	
}

int Input::update (  )
{
	switch (getState())
	{
		case State::FREE:
			if (press)
			{
				pressTime = SDL_GetTicks();
				setState(State::PRESS);
			}
			break;
		
		case State::PRESS:
			if (press)
			{
				setState(State::HOLD);
			}
			else
			{
				setState(State::RELEASE);
			}
			break;
		
		case State::HOLD:
			if (press == false)
				setState(State::RELEASE);
			break;
		
		case State::RELEASE:
			press = false;
			setState(State::FREE);
			break;
		
		default:
			setState(State::FREE);
			break;
	}
	
	return getState();
}

