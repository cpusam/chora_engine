/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef CHORA_COLLISION_HPP
#define CHORA_COLLISION_HPP

#include "sdl.hpp"
#include "vect.hpp"
#include "platform/tilemap.hpp"

#include <vector>


/*
		-1 up
		+1 down
		-2 left
		+2 right
		0 não colide
*/
enum RectSide
{
	NONE=0,
	TOP=-1,
	BOTTOM=1,
	LEFT=-2,
	RIGHT=2,
};

extern inline bool boundingbox ( SDL_Rect a, SDL_Rect b )
{
	if (a.x > b.x + b.w)	return false;
	if (a.x + a.w < b.x)	return false;

	if (a.y > b.y + b.h)	return false;
	if (a.y + a.h < b.y)	return false;

	return true;
}

extern inline bool pointbox ( Vect p, SDL_Rect b )
{
	if (p.x > b.x + b.w)	return false;
	if (p.x < b.x)			return false;

	if (p.y > b.y + b.h)	return false;
	if (p.y < b.y)			return false;

	return true;
}

extern bool lineIntersects (Vect a1, Vect a2, Vect b1, Vect b2, Vect *result=nullptr );

// verifica se "a" está completamente dentro de "b", mas não o contrário.
extern bool rect_inside ( SDL_Rect a, SDL_Rect b );
// retorna o retangulo que é a intersecção de a com b, mas não o contrário.
// se o retorno tiver w e h nulos então não há intersecção
// Tem um bug não resolvido: quando A > B retorna rect errado.
extern SDL_Rect rectIntersect ( SDL_Rect a, SDL_Rect b );

//
extern bool pointtile ( TileMap & map, std::vector <int> & coll_tile, Vect & pos );

// NOTA: REMOVER ESSAS FUNÇÕES QUE NÃO FUNCIONAM!!!!
enum ECollisionMove
{
	NO_MOVE=0,
	MOVE_TO_UP, // mover pos para cima
	MOVE_TO_RIGHT, // mover pos para a direita
	MOVE_TO_DOWN, // mover pos para baixo
	MOVE_TO_LEFT, // mover pos para a esquerda
};

enum ECollisionTileSide
{
	NO_COLLISION=0,
	UP_SIDE,
	RIGHT_SIDE,
	DOWN_SIDE,
	LEFT_SIDE,
};

extern ECollisionTileSide tile_collision ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel, ECollisionMove move );

// colisão horizontal nos coll_tile, aqui é processada colisão no eixo X
extern int collision_hor ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel );

// colisão vertical nos coll_tile, aqui é processada colisão no eixo Y
extern int collision_ver ( TileMap & map, std::vector <int> coll_tile, Vect & pos, std::vector <Vect> c_point, Vect & vel );
#endif
