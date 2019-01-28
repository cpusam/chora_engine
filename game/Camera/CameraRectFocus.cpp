#include "CameraRectFocus.hpp"
#include "../../SDL_gfx/SDL_framerate.hpp"

CameraRectFocus::CameraRectFocus ( SDL_Renderer * renderer, SDL_Rect viewport, SDL_Rect limit, SDL_Rect rectFocus ):
Camera(renderer, viewport.x, viewport.y, viewport.w, viewport.h, limit)
{
	this->rectFocus = rectFocus;
}

void CameraRectFocus::setRectFocus ( SDL_Rect rect )
{
	rectFocus = rect;
}

SDL_Rect CameraRectFocus::getRectFocus (  )
{
	return rectFocus;
}

void CameraRectFocus::lookat ( Vect p )
{
	Vect rect(position.x, position.y);

	if (p.x < rect.x + rectFocus.x)
	{
		position.x = rect.x - ((rect.x + rectFocus.x) - p.x);
	}
	else if (p.x > rect.x + rectFocus.x + rectFocus.w)
	{
		position.x = rect.x + (p.x - (rect.x + rectFocus.x + rectFocus.w));
	}

	if (position.x < limit.x)
		position.x = limit.x;
	else if (position.x + dimension.w > limit.x + limit.w)
		position.x = limit.x + limit.w - dimension.w;
	
	//eixo Y
	if (p.y < rect.y + rectFocus.y)
	{
		position.y = rect.y - ((rect.y + rectFocus.y) - p.y);
	}
	else if (p.y > rect.y + rectFocus.y + rectFocus.h)
	{
		position.y = rect.y + (p.y - (rect.y + rectFocus.y + rectFocus.h));
	}

	if (position.y < limit.y)
		position.y = limit.y;
	else if (position.y + dimension.h > limit.y + limit.h)
		position.y = limit.y + limit.h - dimension.h;
}


bool CameraRectFocus::moveToPosition ( Vect & p, float maxVel )
{
	Vect diff(p);

	diff.normalize();
	float seconds = FPSManager::instance()->getDeltaSeconds();
	velocity.x = diff.x * maxVel * seconds;
	velocity.y = diff.y * maxVel * seconds;

	position.x += velocity.x;
	position.y += velocity.y;

	bool changeX = false, changeY = false;
	if (velocity.x < 0)
		changeX = int(position.x) < int(p.x);
	else if (velocity.x > 0)
		changeX = int(position.x) > int(p.x);

	if (velocity.y < 0)
		changeY = int(position.y) < int(p.y);
	else if (velocity.y > 0)
		changeY = int(position.y) > int(p.y);

	return changeX && changeY;
}