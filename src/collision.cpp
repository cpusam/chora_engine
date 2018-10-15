#include "../include/collision.hpp"

bool lineIntersects ( Vect a1, Vect a2, Vect b1, Vect b2, Vect * result )
{
    Vect intersection;

    Vect b = Vect::sub(a2, a1);
    Vect d = Vect::sub(b2, b1);
    float bDotDPerp = b.x * d.y - b.y * d.x;

		//se b vezes d for 0 então as linhas são paralelas
    if (bDotDPerp == 0)
        return false;

    Vect c = b1 - a1;
    float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
    if (t < 0 || t > 1)
        return false;

    float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
    if (u < 0 || u > 1)
        return false;

		if (result)
    	*result = Vect::add(a1, (b * t));

    return true;
}

// verifica se "a" está completamente dentro de "b", mas não o contrário
bool rect_inside ( SDL_Rect a, SDL_Rect b )
{
	if (a.x + a.w > b.x + b.w)	return false;
	if (a.x < b.x)	return false;

	if (a.y + a.h > b.y + b.h)	return false;
	if (a.y < b.y)	return false;

	return true;
}


SDL_Rect rectIntersect ( SDL_Rect a, SDL_Rect b )
{
	SDL_Rect rect = {0,0,0,0};

	if (a.x < b.x)
	{
		rect.x = b.x;
		if (a.x + a.w > b.x + b.w)
		{
			if (a.w <= b.w)
				rect.w = a.w;
			else
				rect.w = b.w;
		}
		else if ((a.x + a.w) - b.x > 0)
			rect.w = (a.x + a.w) - b.x;
		else
			return (SDL_Rect){0,0,0,0};
	}
	else if (a.x > b.x)
	{
		rect.x = a.x;
		if (a.x + a.w < b.x + b.w)
		{
			if (a.w <= b.w)
				rect.w = a.w;
			else
				rect.w = b.w;
		}
		else if ((b.x + b.w) - a.x > 0)
			rect.w = (b.x + b.w) - a.x;
		else
			return (SDL_Rect){0,0,0,0};
	}
	else
	{
		rect.x = b.x;
		if (a.w >= b.w)
			rect.w = b.w;
		else
			rect.w = a.w;
	}


	if (a.y < b.y)
	{
		rect.y = b.y;
		if (a.y + a.h > b.y + b.h)
		{
			if (a.h <= b.h)
				rect.h = a.h;
			else
				rect.h = b.h;
		}
		else if ((a.y + a.h) - b.y > 0)
			rect.h = (a.y + a.h) - b.y;
		else
			return (SDL_Rect){0,0,0,0};
	}
	else if (a.y > b.y)
	{
		rect.y = a.y;
		if (a.y + a.h < b.y + b.h)
		{
			if (a.h <= b.h)
				rect.h = a.h;
			else
				rect.h = b.h;
		}
		else if ((b.y + b.h) - a.y > 0)
			rect.h = (b.y + b.h) - a.y;
		else
			return (SDL_Rect){0,0,0,0};
	}
	else
	{
		rect.y = b.y;
		if (a.h >= b.h)
			rect.h = b.h;
		else
			rect.h = a.h;
	}

	return rect;
}


bool pointtile ( TileMap & map, std::vector <int> & coll_tile, Vect &  pos )
{
	int tile = map.get_tile(pos.x, pos.y);
	for (unsigned int i = 0, s = coll_tile.size(); i < s; i++)
		if (coll_tile[i] == tile)
			return true;

	return false;
}