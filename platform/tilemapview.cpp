#include "tilemapview.hpp"

#include <algorithm>

////////////////////////////////////////////////////////////////////
CAnimatedTile TileMapView::get_animation ( int tile )
{
	for (unsigned int i = 0; i < animation.size(); i++)
		if (animation.at(i).getTile() == tile)
			return animation.at(i);
	
	return CAnimatedTile();
}

SDL_Rect TileMapView::getSourceRect ( int tile )
{
	if (source.count(tile) > 0)
		return source[tile];
	
	return (SDL_Rect){0,0,-1,-1};
}

void TileMapView::addAnimation ( CAnimatedTile & a, int t )
{
	//if (!texture)
		//throw "TileMapView: texture nula";
	//a.setTexture(texture);

	a.setTile(t);
	addTile(t);
	#ifdef OLD_ANIMATED
	animation[t] = a;
	#else
	if (!isAnimated(t))
	{
		animatedTilesID.push_back(t);
		animatedTiles.push_back(a);
	}
	#endif
}

bool TileMapView::isAnimated (	int t )
{
	#ifdef OLD_ANIMATED
	return (animation.find(t) != animation.end());
	#else
	return (std::find(animatedTilesID.begin(), animatedTilesID.end(), t) != animatedTilesID.end());
	#endif
}

void TileMapView::updateAnimation (  )
{
	#ifdef OLD_ANIMATED
	for (std::vector <int>::iterator i = tiles.begin(); i != tiles.end(); i++)
	{
		if (isAnimated(*i))
			animation[*i].update();
	}
	#else
	for (auto & it: animatedTiles)
		it.update();
	#endif
}



int TileMapView::draw ( SDL_Renderer * renderer, Camera * cam )
{
	if (!texture)
		return -1;

	int i, j, t, ret = 0;
	Vect position = cam->getPosition() - this->position, pCam = cam->getPosition() - this->position;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = cam->getDimension();

	Vect p;
	p.x = int(position.x) / tilesize;
	p.y = int(position.y) / tilesize;
	dim.x = 0;
	dim.y = 0;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(pCam.x) % tilesize;
	int mod_y = int(pCam.y) % tilesize;
	int maxHor = p.x + dim.w + 1;
	int maxVer = p.y + dim.h + 1;

	//para o caso do mapa ser menor que as dimensões da camera
	if (width < dim.w + 1)
	{
		mod_x = 0;
		maxHor -= 1;
	}

	if (height < dim.h + 1)
	{
		mod_y = 0;
		maxVer -= 1;
	}
	
	if (!tileset.size())
	{
		std::cout<<"TileMapView::Error não tem tiles\n";
		return -1;
	}

	for (i = p.x - 1; i <= maxHor; i++)
		for (j = p.y - 1; j <= maxVer; j++)
		{
			int index = j * width + i;
			if (index < 0 || index >= int(tileset.size()))
				continue;
			
			t = tileset[index];
			//t = getTile(i * tilesize, j * tilesize);
			if (!hasTile(t))
				continue;

			dest.x = ((i - p.x) * tilesize + dim.x) - mod_x;
			dest.y = ((j - p.y) * tilesize + dim.y) - mod_y; 

			dest.w = tilesize;
			dest.h = tilesize;

			if (isAnimated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].getCurrentFrame();
						src = frame.getSourceRect();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].getTexture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].getAngle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			src = source[t];
			ret = SDL_RenderCopy(renderer, texture, &src, &dest);
		
			if (ret < 0)
				break;
		}
	
	return ret;
}

int TileMapView::draw ( SDL_Renderer * renderer, Camera * cam, int x, int y )
{
	if (!texture)
		return -1;
	
	int i, j, t, ret = 0;
	Vect position = cam->getPosition() - this->position;
	Vect p = cam->getPosition() - this->position;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, cam->getDimension().w, cam->getDimension().h};


	// limita o movimento da camera
	if (dim.w > width)
		dim.w = width;

	if (dim.h > height)
		dim.h = height;

	if (position.x < 0)
		position.x = 0;
	else if (position.x + dim.w > width)
		position.x = width - dim.w;

	if (position.y < 0)
		position.y = 0;
	else if (position.y + dim.h > height)
		position.y = height - dim.h;


	position.x = int(position.x) / tilesize;
	position.y = int(position.y) / tilesize;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(p.x) % tilesize;
	int mod_y = int(p.y) % tilesize;



	for (i = position.x - 1; i <= position.x + dim.w + 1; i++)
		for (j = position.y - 1; j <= position.y + dim.h + 1; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = getTile(i * tilesize, j * tilesize);
			if (!hasTile(t))
				continue;

			//é preciso parenteses extras para evitar bugs
			dest.x = ((i - position.x) * tilesize + dim.x + x) - mod_x;
			dest.y = ((j - position.y) * tilesize + dim.y + y) - mod_y;

			dest.w = tilesize;
			dest.h = tilesize;

			if (isAnimated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].getCurrentFrame();
						src = frame.getSourceRect();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].getTexture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].getAngle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			{
				src = source[t];
				ret = SDL_RenderCopy(renderer, texture, &src, &dest);
			}
			
			if (ret < 0)
				break;
			
				break;

		}
	
	return ret;
}

int TileMapView::draw ( SDL_Renderer * renderer, int x, int y )
{
	if (!texture)
		return -1;
	
	int i, j, t, ret = 0;
	Vect position, p;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, width, height};


	dim.w /= tilesize;
	dim.h /= tilesize;

	for (i = -1; i <= dim.w+1; i++)
		for (j = -1; j <= dim.h+1; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = getTile(i * tilesize, j * tilesize);
			if (!hasTile(t))
				continue;
			
			dest.x += x;
			dest.y += y;

			dest.w = tilesize;
			dest.h = tilesize;
			
			if (isAnimated(t))
			{
				#ifdef OLD_ANIMATED
					animation[t].draw(renderer, dest.x, dest.y);
				#else
				for (int k = 0; k < animatedTilesID.size(); k++)
					if (animatedTilesID[k] == t)
					{
						AnimationFrame frame = animatedTiles[k].getCurrentFrame();
						src = frame.getSourceRect();
						SDL_Point center = {src.w/2, src.h/2};
						SDL_Texture * textureAnim = animatedTiles[k].getTexture();
						ret = SDL_RenderCopyEx(renderer, textureAnim, &src, &dest, animatedTiles[k].getAngle(), &center, frame.get_flip());
						break;
					}
				#endif
				continue;
			}

			//if (texture)
			{
				src = source[t];
				ret = SDL_RenderCopy(renderer, texture, &src, &dest);
			}
			
			if (ret < 0)
				break;
		}
	
	return ret;
}


