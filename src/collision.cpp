#include "collision.hpp"

bool boundingbox ( SDL_Rect a, SDL_Rect b )
{
	if (a.x > b.x + b.w)	return false;
	if (a.x + a.w < b.x)	return false;

	if (a.y > b.y + b.h)	return false;
	if (a.y + a.h < b.y)	return false;

	return true;
}

bool pointbox ( Vect p, SDL_Rect b )
{
	if (p.x > b.x + b.w)	return false;
	if (p.x < b.x)			return false;

	if (p.y > b.y + b.h)	return false;
	if (p.y < b.y)			return false;

	return true;
}


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

bool has_coll_tile ( std::vector <int> coll_tile, int tile )
{
	for (unsigned int i = 0, s = coll_tile.size(); i < s; i++)
		if (coll_tile[i] == tile)
			return true;

	return false;
}

ECollisionTileSide tile_collision ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel, ECollisionMove move )
{
	ECollisionTileSide ret = NO_COLLISION;
	float p;
	Vect cp;
	SDL_Rect d = map.get_dimension();

	for (unsigned int i = 0, s = c_point.size(); i < s; i++)
	{
		//printf("%s %d observe não é para eixo Y somente para X\n", __FILE__, __LINE__);
		cp.x = pos.x + c_point[i].x + vel.x;
		cp.y = pos.y + c_point[i].y;

		// verifica se está dentro do mapa
		if (!pointbox(cp, d))
			continue;

		//
		if (has_coll_tile(coll_tile, map.get_tile(cp.x, cp.y)))
		{
			if (move == MOVE_TO_LEFT)
			{
				// colisão à direita
				p = int(pos.x + c_point[i].x + vel.x) / map.get_tilesize();
				p *= map.get_tilesize();
				pos.x = p - c_point[i].x - 1;
				vel.x = 0;
				ret = RIGHT_SIDE;
			}
			else if (move == MOVE_TO_RIGHT)
			{
				// colisão à esquerda
				p = int(pos.x + c_point[i].x + vel.x) / map.get_tilesize();
				p += 1;
				p *= map.get_tilesize();
				pos.x = p - c_point[i].x;
				vel.x = 0;
				ret = LEFT_SIDE;
			}
			else if (move == MOVE_TO_UP)
			{
				// colisão em baixo
				p = int(pos.y + c_point[i].y + vel.y) / map.get_tilesize();
				p *= map.get_tilesize();
				pos.y = p - c_point[i].y - 1;
				vel.y = 0;
				ret = DOWN_SIDE;
			}
			else if (move == MOVE_TO_DOWN)
			{
				// colisão em cima
				p = int(pos.y + c_point[i].y + vel.y) / map.get_tilesize();
				p += 1;
				p *= map.get_tilesize();
				pos.y = p - c_point[i].y;
				vel.y = 0;
				ret = UP_SIDE;
			}
			break;
		}
	}

	return ret;
}

int collision_hor ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel )
{
	int ret = 0;
	float p;
	Vect cp;
	SDL_Rect d = map.get_dimension();

	if (vel.x == 0)
		return 0;

	for (unsigned int i = 0, s = c_point.size(); i < s; i++)
	{
		cp.x = pos.x + c_point[i].x + vel.x;
		cp.y = pos.y + c_point[i].y;

		// verifica se está dentro do mapa
		if (!pointbox(cp, d))
			continue;

		//
		if (has_coll_tile(coll_tile, map.get_tile(cp.x, cp.y)))
		{
			if (vel.x > 0)
			{
				// colisão à direita
				p = int(pos.x + c_point[i].x + vel.x) / map.get_tilesize();
				p *= map.get_tilesize();
				pos.x = p - c_point[i].x - 1;
				vel.x = 0;
				ret = 1;
			}
			else
			{
				// colisão à esquerda
				p = int(pos.x + c_point[i].x + vel.x) / map.get_tilesize();
				p += 1;
				p *= map.get_tilesize();
				pos.x = p - c_point[i].x;
				vel.x = 0;
				ret = -1;
			}
			break;
		}
	}

	return ret;
}

int collision_ver ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel )
{
	int ret = 0;
	float p;
	Vect cp;
	SDL_Rect d = map.get_dimension();

	if (vel.y == 0)
		return ret;

	for (unsigned int i = 0, s = c_point.size(); i < s; i++)
	{
		cp.x = pos.x + c_point[i].x;
		cp.y = pos.y + c_point[i].y + vel.y;

		// verifica se está dentro do mapa
		if (!pointbox(cp, d))
			continue;

		//
		if (has_coll_tile(coll_tile, map.get_tile(cp.x, cp.y)))
		{
			if (vel.y > 0)
			{
				// colisão em baixo
				p = int(pos.y + c_point[i].y + vel.y) / map.get_tilesize();
				p *= map.get_tilesize();
				pos.y = p - c_point[i].y - 1;
				vel.y = 0;
				ret = 1;
			}
			else // já é vel.y < 0
			{
				// colisão em cima
				p = int(pos.y + c_point[i].y + vel.y) / map.get_tilesize();
				p += 1;
				p *= map.get_tilesize();
				pos.y = p - c_point[i].y;
				vel.y = 0;
				ret = -1;
			}
			break;
		}
	}

	return ret;
}
