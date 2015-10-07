#include "statemachine.hpp"

void StateMachine::set_state ( int s )
{
	state = s;
}

int StateMachine::get_state (  )
{
	return state;
}

int StateMachine::update (  )
{
	return get_state();
}


//////////////////////////////////////////////////////////////////
bool StatePatternMachine::have_state ( CState * s )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i] == s)
			return true;

	return false;
}

bool StatePatternMachine::add_state ( CState * s )
{
	if (!have_state(s))
	{
		state.push_back(s);
		return true;
	}

	return false;
}

bool StatePatternMachine::set_state ( int id )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
		{
			curr_state = state[i];
			return true;
		}

	return false;
}

CState * StatePatternMachine::get_state ( int id )
{
	for (int i = 0; i < state.size(); i++)
		if (state[i]->get_id() == id)
			return state[i];

	return 0;
}

CState * StatePatternMachine::get_curr_state (  )
{
	return curr_state;
}

int StatePatternMachine::update (  )   // retorna o id do estado
{
	return DEFAULT_STATE_ID;
}
