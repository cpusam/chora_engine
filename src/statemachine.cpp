#include "statemachine.hpp"

void StateMachine::setState ( int s )
{
	state = s;
}

int StateMachine::getState (  )
{
	return state;
}

void StateMachine::input ( SDL_Event & event )
{
}

int StateMachine::update (  )
{
	return getState();
}
