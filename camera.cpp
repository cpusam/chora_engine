#include "camera.hpp"
#include "collision.hpp"

Camera::Camera ( SDL_Renderer * renderer, int x, int y, int vpW, int vpH, SDL_Rect l )
{
	dimension = (SDL_Rect){0,0,vpW,vpH};
	viewport = (SDL_Rect){x,y,vpW,vpH};
	updateViewport(renderer);
	limit = l;
	SDL_RenderSetViewport(renderer, &viewport);
	// focus padrão no meio da camera
	focus.x = dimension.w / 2.0f;
	focus.y = dimension.h / 2.0f;
	//lookat(Vect(0,0));
	if (limit.w < dimension.w || limit.h < dimension.h)
		std::cout << "Camera: atenção limit W ou H menor que dimension W ou H\n";

	action_area = (SDL_Rect){0,0,dimension.w,dimension.h};
}

Camera::Camera ( SDL_Rect d, SDL_Rect l )
{
	dimension = d;
	limit = l;
	// focus padrão no meio da camera
	focus.x = d.w / 2.0f;
	focus.y = d.h / 2.0f;
	//lookat(Vect(0,0));
	if (limit.w < dimension.w || limit.h < dimension.h)
		std::cout << "Camera: atenção limit W ou H menor que dimension W ou H\n";

	action_area = (SDL_Rect){0,0,d.w,d.h};
}

SDL_Rect Camera::getViewport (  )
{
	return viewport;
}

void Camera::updateViewport ( SDL_Renderer * renderer )
{
	SDL_RenderSetViewport(renderer, &viewport);
}

void Camera::lookat ( Vect p )
{
	// antes aqui tinha um bug e só funcionava para eixos positivos
	//position.x = p.x - focus.x;
	//position.y = p.y - focus.y;
	position = p - focus;

	if (position.x < limit.x)
		position.x = limit.x;
	else if (position.x + dimension.w > limit.x + limit.w)
		position.x = (limit.x + limit.w) - dimension.w;

	if (position.y < limit.y)
		position.y = limit.y;
	else if (position.y + dimension.h > limit.x + limit.h)
		position.y = (limit.y + limit.h) - dimension.h;
	
	SDL_Rect d = action_area;
	if (d.w > dimension.w)
		d.x = position.x - ((d.w/2) - (dimension.w/2));
	else if (d.w < dimension.w)
		d.x = position.x - ((dimension.w/2) - (d.w/2));
	
	if (d.h > dimension.h)
		d.y = position.y - ((d.h/2) - (dimension.h/2));
	else if (d.h < dimension.h)
		d.y = position.y - ((dimension.h/2) - (d.h/2));
	
	setActionArea(d);
}

Vect Camera::get_position (  )
{
	return position;
}

Vect Camera::get_focus (  )
{
	return focus;
}

int Camera::set_focus ( Vect f )
{
	if (f.x < 0 || f.y < 0)
		return -1;

	if (f.x > dimension.w || f.y > dimension.h)
		return -2;

	focus = f;
	return 0;
}

SDL_Rect Camera::get_view (  )
{
	return (SDL_Rect){int(position.x), int(position.y), dimension.w,dimension.h};
}

void Camera::set_position ( Vect p )
{
	position = p;

	if (position.x < limit.x)
		position.x = limit.x;
	else if (position.x + dimension.w > limit.x + limit.w)
		position.x = limit.w - dimension.w;

	if (position.y < limit.y)
		position.y = limit.y;
	else if (position.y + dimension.h > limit.x + limit.h)
		position.y = limit.h - dimension.h;
	
}

SDL_Rect Camera::get_dimension (  )
{
	return dimension;
}

SDL_Rect Camera::get_limit (  )
{
	return limit;
}

void Camera::setScreenPos ( int x, int y )
{
	viewport.x = x;
	viewport.y = y;
}

void Camera::set_limit ( SDL_Rect l )
{
	limit = l;
}

bool Camera::inAction ( Vect p )
{
	return pointbox(p, action_area);
}

bool Camera::inAction ( SDL_Rect d )
{
	return boundingbox(d, action_area);
}

void Camera::setActionArea ( SDL_Rect d )
{
	action_area = d;
}

SDL_Rect Camera::getActionArea (  )
{
	return action_area;
}

