#include "movable.hpp"

void Movable::set_dir ( int d )
{
	dir = d;
}

int Movable::get_dir (  )
{
	return dir;
}

void Movable::set_pos ( Vect p )
{
	pos = p;
}

Vect Movable::get_pos (  )
{
	return pos;
}

void Movable::setPosition ( Vect p )
{
	pos = p;
}

Vect Movable::getPosition (  )
{
	return pos;
}

void Movable::set_vel ( Vect v )
{
	vel = v;
}

Vect Movable::get_vel (  )
{
	return vel;
}

void Movable::set_acc ( Vect a )
{
	acc = a;
}

Vect Movable::get_acc (  )
{
	return acc;
}

void Movable::set_dim ( SDL_Rect d )
{
	dim = d;
}

SDL_Rect Movable::get_dim (  )
{
	return dim;
}


