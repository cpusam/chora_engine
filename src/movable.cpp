#include "movable.hpp"
#include "SDL_gfx/SDL_framerate.hpp"

Vect Movable::getAcceleration (  )
{
	return acc;
}

float Movable::getAccelerationX (  )
{
	return acc.x;
}

float Movable::getAccelerationY (  )
{
	return acc.y;
}


Vect Movable::getPosition (  )
{
	return position;
}

float Movable::getPositionX (  )
{
	return position.x;
}

float Movable::getPositionY (  )
{
	return position.y;
}


Vect Movable::get_vel (  )
{
	return vel;
}

float Movable::get_vel_x (  )
{
	return vel.x;
}

float Movable::get_vel_y (  )
{
	return vel.y;
}

void Movable::setAcceleration ( Vect a )
{
	acc = a;
}

void Movable::setAccelerationX ( float x )
{
	acc.x = x;
}

void Movable::setAccelerationY ( float y )
{
	acc.y = y;
}


void Movable::setPosition ( Vect p )
{
	position = p;
}

void Movable::setPositionX ( float px )
{
	position.x = px;
}

void Movable::setPositionY ( float py )
{
	position.y = py;
}

void Movable::setVelocity ( Vect v )
{
	vel = v;
}

void Movable::setVelocityX ( float x )
{
	vel.x = x;
}

void Movable::setVelocityY ( float y )
{
	vel.y = y;
}


void Movable::setDimension ( SDL_Rect d )
{
	dim = d;
}

SDL_Rect Movable::getDimension (  )
{
	return dim;
}

