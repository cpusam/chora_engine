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

#ifndef CHORA_TILEMAP_HPP
#define CHORA_TILEMAP_HPP

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "../sdl.hpp"
#include "../include/vect.hpp"
#include "../include/camera.hpp"
#include "../include/animation.hpp"

class TileMap
{
	protected:
		int width; // largura do std::mapa
		int height;
		int tilesize;
		Vect position;
		SDL_Rect dimension;
		std::vector <int> tileset;//conjunto de tiles do mapa
		std::vector <int> tiles;//tiles neste mapa

	public:
		TileMap ( int ts )
		{
			width = height = 1;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,tilesize,tilesize};
		}

		TileMap ( int ts, int w, int h )
		{
			width = w;
			height = h;
			tilesize = ts;
			dimension = (SDL_Rect){0,0,tilesize * width,tilesize * height};
		}

		void setTilesize ( int ts );

		void setPosition ( Vect p );
		
		Vect getPosition (  );

		bool setTile ( int x, int y, int t );

		void setTile ( int i, int t );

		inline int getTile ( int x, int y )
		{
			if (x < 0 || y < 0)
			{
				static int p = 0;
				if (p < 100)
				{
					printf("CTilaMap: getTile posições negativas x = %d, y = %d\n", x, y);
				}
				p++;
				//throw "TileMap: getTile posições negativas!\n";
				return -1;
			}

			x = int((x - position.x) / tilesize);
			y = int((y - position.y) / tilesize);

			if (y >= height || x >= width)
				return -1;

			if (y * width + x < int(tileset.size()))
			{
				//if (x < width && y < height)
				return tileset[y * width + x];
			}

			return -1;
		}
		
		//pega a posição em pixels
		Vect get_tile_pos ( int i );

		// Se o indice i for um tile, retorna >= 0. Caso contrário < 0 caso não haja
		int getTile ( int i );

		int getTilesize (  );

		int getWidth (  );

		int getHeight (  );

		SDL_Rect getDimension (  );

		bool hasTile ( int t );

		bool addTile ( int t );

		void removeTile ( int t );

		int findFirstPosition ( int t );

		/*
			Lẽ todo o vetor e se encontra um -1 soma uma linha
		*/
		int read ( std::vector <int> & t );
		/*
			cada linha de tileset está dividida com -1, ou seja,
			a cada tile sendo -1 é uma linha.
		*/
		int read ( int * t, int size );

		int read ( std::string filename );

		//lê um mapa de um xpm
		int readFromXPM ( std::string filename );
};


#endif


