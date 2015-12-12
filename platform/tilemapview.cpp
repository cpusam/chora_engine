#include "tilemapview.hpp"

////////////////////////////////////////////////////////////////////
void TileMapView::add_animation ( CAnimatedTile a, int t )
{
	if (!texture)
		throw "TileMapView: texture nula";
	a.set_texture(texture);

	a.set_tile(t);
	add_tile(t);
	animation[t] = a;
}

bool TileMapView::is_animated (	int t )
{
	return (animation.count(t) > 0);
}

void TileMapView::update_animation (  )
{
	for (std::vector <int>::iterator i = tiles.begin(); i != tiles.end(); i++)
	{
		if (is_animated(*i))
			animation[*i].update();
	}
}



void TileMapView::draw ( SDL_Renderer * renderer, Camera * cam )
{
	int i, j, t;
	Vect pos = cam->get_position(), p = cam->get_position();
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = cam->get_dimension();

	pos.x = int(pos.x) / tilesize;
	pos.y = int(pos.y) / tilesize;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(p.x) % tilesize;
	int mod_y = int(p.y) % tilesize;

	for (i = pos.x; i <= pos.x + dim.w; i++)
		for (j = pos.y; j <= pos.y + dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				animation[t].draw(renderer, cam, i * tilesize, j * tilesize);
				continue;
			}

			src = source[t];

			if (i == pos.x)
			{
				src.x += mod_x;
				src.w = tilesize - mod_x;
				dest.x = dim.x;
			}
			else if (i == pos.x + dim.w)
			{
				if (mod_x == 0)
					continue;

				src.w = mod_x;
				dest.x = dim.x + dim.w * tilesize - src.w;
			}
			else
			{
				dest.x = dim.x + (i - pos.x) * tilesize - (mod_x);
			}

			if (j == pos.y)
			{
				src.y += mod_y;
				src.h = tilesize - mod_y;
				dest.y = dim.y;
			}
			else if (j == pos.y + dim.h)
			{
				if (mod_y == 0)
					continue;

				src.h = mod_y;
				dest.y = dim.y + dim.h * tilesize - src.h;
			}
			else
			{
				dest.y = dim.y + (j - pos.y) * tilesize - (mod_y);
			}

			if (texture)
			{
				dest.w = src.w;
				dest.h = src.h;
				SDL_RenderCopy(renderer, texture, &src, &dest);
			}
		}
}

void TileMapView::draw ( SDL_Renderer * renderer, SDL_Rect cam, int x, int y )
{
	int i, j, t;
	Camera *test = new Camera(cam, cam);
	Vect pos = Vect(cam.x, cam.y), p = Vect(cam.x, cam.y);
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, cam.w, cam.h};


	// limita o movimento da camera
	if (dim.w > width)
		dim.w = width;

	if (dim.h > height)
		dim.h = height;

	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x + dim.w > width)
		pos.x = width - dim.w;

	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y + dim.h > height)
		pos.y = height - dim.h;


	pos.x = int(pos.x) / tilesize;
	pos.y = int(pos.y) / tilesize;
	dim.w /= tilesize;
	dim.h /= tilesize;
	int mod_x = int(p.x) % tilesize;
	int mod_y = int(p.y) % tilesize;



	for (i = pos.x; i <= pos.x + dim.w; i++)
		for (j = pos.y; j <= pos.y + dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				animation[t].draw(renderer,test,i * tilesize, j * tilesize);
			}

			src = source[t];

			if (i == pos.x)
			{
				src.x += mod_x;
				src.w = tilesize - mod_x;
				dest.x = dim.x;
			}
			else if (i == pos.x + dim.w)
			{
				if (mod_x == 0)
					continue;

				src.w = mod_x;
				dest.x = dim.x + dim.w * tilesize - src.w;
			}
			else
			{
				dest.x = dim.x + (i - pos.x) * tilesize - (mod_x);
			}

			if (j == pos.y)
			{
				src.y += mod_y;
				src.h = tilesize - mod_y;
				dest.y = dim.y;
			}
			else if (j == pos.y + dim.h)
			{
				if (mod_y == 0)
					continue;

				src.h = mod_y;
				dest.y = dim.y + dim.h * tilesize - src.h;
			}
			else
			{
				dest.y = dim.y + (j - pos.y) * tilesize - (mod_y);
			}

			if (texture)
			{

				dest.x += x;
				dest.y += y;
				dest.w = src.w;
				dest.h = src.h;
				SDL_RenderCopy(renderer, texture, &src, &dest);
			}
		}
}

void TileMapView::draw ( SDL_Renderer * renderer, int x, int y )
{
	int i, j, t;
	Vect pos, p;
	SDL_Rect dest = {0,0,tilesize,tilesize};
	SDL_Rect src = {0,0,0,0};
	SDL_Rect dim = {0,0, width, height};


	dim.w /= tilesize;
	dim.h /= tilesize;

	for (i = 0; i <= dim.w; i++)
		for (j = 0; j <= dim.h; j++)
		{
			//t = tileset.at(i * width + j); <- BUGADO!
			t = get_tile(i * tilesize, j * tilesize);
			if (!has_tile(t))
				continue;

			if (is_animated(t))
			{
				animation[t].draw(renderer, i * tilesize, j * tilesize);
			}

			src = source[t];

			if (texture)
			{
				dest.x += x;
				dest.y += y;
				dest.w = tilesize;
				dest.h = tilesize;
				SDL_RenderCopy(renderer, texture, &src, &dest);
			}
		}
}


