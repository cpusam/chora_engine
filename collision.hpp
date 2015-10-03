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

extern bool boundingbox ( SDL_Rect a, SDL_Rect b );
extern bool pointbox ( SVect p, SDL_Rect b );

// verifica se "a" está completamente dentro de "b", mas não o contrário
extern bool rect_inside ( SDL_Rect a, SDL_Rect b );

// 
extern bool pointtile ( CTileMap & map, std::vector <int> & coll_tile, SVect & pos );

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

extern ECollisionTileSide tile_collision ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel, ECollisionMove move );

// colisão horizontal nos coll_tile, aqui é processada colisão no eixo X
extern int collision_hor ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel );

// colisão vertical nos coll_tile, aqui é processada colisão no eixo Y
extern int collision_ver ( CTileMap & map, std::vector <int> coll_tile, SVect & pos, std::vector <SVect> c_point, SVect & vel );
#endif


