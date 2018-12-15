#ifndef CHORA_TILEMAPVIEW_HPP
#define CHORA_TILEMAPVIEW_HPP

#include "../include/animation.hpp"
#include "tilemap.hpp"

#define OLD_ANIMATED

class CAnimatedTile: public Animation
{
	protected:
		int tile;

	public:
		CAnimatedTile (  )
		{
			tile = -1;
		}

		SDL_Texture * getTexture (  )
		{
			return getCurrentFrame().getTexture();
		}

		int getTile (  )
		{
			return tile;
		}

		void setTile ( int t )
		{
			tile = t;
		}
};

class TileMapView: public TileMap
{
	private:
		std::vector<int> animatedTilesID;
		std::vector<CAnimatedTile> animatedTiles;
	protected:
		std::map <int, SDL_Rect> source; // par <tile, rect_source>
		std::map <int, CAnimatedTile> animation; // par <tile, animação de tile>
	public:
		SDL_Texture * texture;

	public:
		TileMapView ( int tilesize, SDL_Texture * t=nullptr ): TileMap(tilesize)
		{
			texture = t;
		}
		~TileMapView (  )
		{
		}
		
		CAnimatedTile get_animation ( int tile );
		SDL_Rect getSourceRect ( int tile );
		
		void clearSource (  )
		{
			source.clear();
		}

		void setSourceRect ( int k, SDL_Rect f )
		{
			source[k] = f;
		}

		void addAnimation ( CAnimatedTile & a, int t );

		bool isAnimated ( int t );

		void updateAnimation (  );

		// desenha o mapa na posição 0,0
		int draw ( SDL_Renderer * renderer, Camera * cam );
		// desenha o mapa numa posição e usando uma camera
		int draw ( SDL_Renderer * renderer, Camera * cam, int x, int y );
		// desenha todo o mapa numa posição
		int draw ( SDL_Renderer * renderer, int x, int y );
};

#endif
