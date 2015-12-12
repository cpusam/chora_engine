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
	
	if (a.w * a.h < b.w * b.h)
		return (SDL_Rect){0,0,0,0};
	
	if (boundingbox(a,b) == false)
		return (SDL_Rect){0,0,0,0};
	
	if (rect_inside(a,b))
		return a;
	
	if (rect_inside(b,a))
		return b;
	
	if (a.x > b.x && a.x < b.x + b.w)
	{
		rect.x = a.x;
		rect.w = (b.x + b.w) - a.x;
	
		if (rect.w < 0)
			return (SDL_Rect){0,0,0,0};
	}
	else if (a.x + a.w > b.x && a.x + a.w < b.x + b.w)
	{
		rect.x = b.x;
		rect.w = (a.x + a.w) - b.x;
	
		if (rect.w < 0)
			return (SDL_Rect){0,0,0,0};
	}
	else 
	{
		if ((a.x <= b.x) && (b.x + b.w <= a.x + a.w))
		{
			rect.x = b.x;
			rect.w = b.w;
		}
	}
	
	if (a.y > b.y  && a.y < b.y + b.h)
	{
		rect.y = a.y;
		rect.h = (b.y + b.h) - a.y;
		if (rect.h < 0)
			return (SDL_Rect){0,0,0,0};
	}
	else if (a.y + a.h > b.y && a.y + a.h < b.y + b.h)
	{
		rect.y = b.y;
		rect.h = (a.y + a.h) - b.y;
		if (rect.h < 0)
			return (SDL_Rect){0,0,0,0};
	}
	else 
	{
		if ((a.y <= b.y) && (b.y + b.h <= a.y + a.h))
		{
			rect.y = b.y;
			rect.h = b.h;
		}
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

