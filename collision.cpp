#include "collision.hpp"

bool boundingbox ( SDL_Rect a, SDL_Rect b )
{
	if (a.x > b.x + b.w - 1)	return false;
	if (a.x + a.w < b.x)	return false;

	if (a.y > b.y + b.h - 1)	return false;
	if (a.y + a.h < b.y)	return false;

	return true;
}

bool pointbox ( SVect p, SDL_Rect b )
{
	if (p.x > b.x + b.w - 1)	return false;
	if (p.x < b.x)			return false;

	if (p.y > b.y + b.h - 1)	return false;
	if (p.y < b.y)			return false;

	return true;
}

// verifica se "a" está completamente dentro de "b", mas não o contrário
bool rect_inside ( SDL_Rect a, SDL_Rect b )
{
	if (a.x + a.w - 1 > b.x + b.w - 1)	return false;
	if (a.x < b.x)	return false;

	if (a.y + a.h - 1 > b.y + b.h - 1)	return false;
	if (a.y < b.y)	return false;

	return true;
}

bool pointtile ( CTileMap & map, std::vector <int> & coll_tile, SVect &  pos )
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

ECollisionTileSide tile_collision ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel, ECollisionMove move )
{
	ECollisionTileSide ret = NO_COLLISION;
	float p;
	SVect cp;
	SDL_Rect d = map.get_dimension();

	for (unsigned int i = 0, s = c_point.size(); i < s; i++)
	{
		printf("%s %d observe não é para eixo Y somente para X\n", __FILE__, __LINE__);
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

int collision_hor ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel )
{
	int ret = 0;
	float p;
	SVect cp;
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

int collision_ver ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel )
{
	int ret = 0;
	float p;
	SVect cp;
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

