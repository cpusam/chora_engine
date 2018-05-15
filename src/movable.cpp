#include "movable.hpp"
#include "SDL_gfx/SDL_framerate.hpp"

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

void Movable::moveX (  )
{
	vel.x = acc.x * FPSManager::instance()->get_delta() / 1000.0f;
	pos.x += vel.x;
}

void Movable::moveY (  )
{
	vel.y = acc.y * FPSManager::instance()->get_delta() / 1000.0f;
	pos.y += vel.y;
}


