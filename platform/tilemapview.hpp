#ifndef CHORA_TILEMAPVIEW_HPP
#define CHORA_TILEMAPVIEW_HPP

#include "../animation.hpp"
#include "tilemap.hpp"

class CAnimatedTile: public Animation
{
	protected:
		int tile;

	public:
		CAnimatedTile (  )
		{
			tile = -1;
			texture.push_back(0);
		}

		SDL_Texture * get_texture (  )
		{
			return texture[0];
		}

		void set_texture ( SDL_Texture * t )
		{
			if (texture[0] && texture[0] != t)
				SDL_DestroyTexture(texture[0]);

			texture[0] = t;
		}

		int get_tile (  )
		{
			return tile;
		}

		void set_tile ( int t )
		{
			tile = t;
		}
};

class TileMapView: public TileMap
{
	protected:
		std::map <int, SDL_Rect> source; // par <tile, rect_source>
		std::map <int, CAnimatedTile> animation; // par <tile, animação de tile>
	public:
		SDL_Texture * texture;

	public:
		TileMapView ( int tilesize, SDL_Texture * t=0 ): TileMap(tilesize)
		{
			texture = t;
		}
		~TileMapView (  )
		{
			if (texture)
				SDL_DestroyTexture(texture);
		}

		void clear_source (  )
		{
			source.clear();
		}

		void set_source ( int k, SDL_Rect f )
		{
			source[k] = f;
		}

		void add_animation ( CAnimatedTile a, int t );

		bool is_animated ( int t );

		void update_animation (  );

		// desenha o mapa na posição 0,0
		void draw ( SDL_Renderer * renderer, Camera * cam );
		// desenha todo o mapa numa posição
		void draw ( SDL_Renderer * renderer, int x, int y );
		// desenha o mapa numa posição e usando uma camera
		void draw ( SDL_Renderer * renderer, Camera * cam, int x, int y );
};

#endif
