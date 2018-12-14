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


//////////////////////////////////////////////////////////////////
bool StatePatternMachine::has_state ( State * s )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i] == s)
			return true;

	return false;
}

bool StatePatternMachine::add_state ( State * s )
{
	if (!has_state(s))
	{
		state.push_back(s);
		return true;
	}

	return false;
}

bool StatePatternMachine::setState ( int id )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i]->getID() == id)
		{
			curr_state = state[i];
			return true;
		}

	return false;
}

State * StatePatternMachine::getState ( int id )
{
	for (unsigned int i = 0; i < state.size(); i++)
		if (state[i]->getID() == id)
			return state[i];

	return 0;
}

State * StatePatternMachine::get_curr_state (  )
{
	return curr_state;
}

int StatePatternMachine::update (  )   // retorna o id do estado
{
	return DEFAULT_STATE_ID;
}
