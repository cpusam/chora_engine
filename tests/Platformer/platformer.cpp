#include "Chora.hpp"
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

enum EPlayerState
{
	STANDING,
	WALKING,
	JUMPING,
};

enum EPlayerDir
{
	LEFT_DIR,
	RIGHT_DIR,
};

class Tux: public StateMachine, public Movable {
		vector <Animation *> anim;
		Animation * curr_anim;
		bool key_right, key_left, key_up;
		int dir;
		TileMap * map;
		vector <Vect> point_up;
		vector <Vect> point_down;
		vector <Vect> point_left;
		vector <Vect> point_right;
		vector <int> coll_tiles;
		float gravity;

public:
		Tux ( SDL_Renderer * renderer, TileMap * m )
		{
			SDL_Surface * aux = NULL;
			SDL_Texture *texture = NULL;
			map = m;
			char buf1[] = {"tux_right.png"};
			char buf2[] = {"tux_left.png"};
			aux = IMG_Load(buf1);
			if (!aux)
			{
				throw SDL_GetError();
			}
			texture = SDL_CreateTextureFromSurface(renderer,aux);
			SDL_FreeSurface(aux);
			aux = NULL;
			// animações de direita
			anim.push_back(new Animation());
			anim[0]->add_frame(texture,(SDL_Rect){144,0,48,43}, 60);
			anim.push_back(new Animation());
			anim[1]->add_frame(texture, (SDL_Rect){0 ,0,48,43}, 3);
			anim[1]->add_frame(texture, (SDL_Rect){48,0,48,43}, 3);
			anim[1]->add_frame(texture, (SDL_Rect){96,0,48,43}, 3);
			anim[1]->add_frame(texture, (SDL_Rect){48,0,48,43}, 3);
			anim.push_back(new Animation());
			anim[2]->add_frame(texture, (SDL_Rect){192,0,48,43}, 3);

			aux = IMG_Load(buf2);
			if (!aux)
			{
				throw SDL_GetError();
			}
			texture = SDL_CreateTextureFromSurface(renderer,aux);
			SDL_FreeSurface(aux);
			aux = NULL;
			// animações de esquerda
			anim.push_back(new Animation());
			anim[3]->add_frame(texture,(SDL_Rect){144,0,48,43}, 60);
			anim.push_back(new Animation());
			anim[4]->add_frame(texture, (SDL_Rect){0 ,0,48,43}, 3);
			anim[4]->add_frame(texture, (SDL_Rect){48,0,48,43}, 3);
			anim[4]->add_frame(texture, (SDL_Rect){96,0,48,43}, 3);
			anim[4]->add_frame(texture, (SDL_Rect){48,0,48,43}, 3);
			anim.push_back(new Animation());
			anim[5]->add_frame(texture, (SDL_Rect){192,0,48,43}, 3);
			
			
			coll_tiles.push_back('a');

			curr_anim = anim[0];
			dir = RIGHT_DIR;
			key_right = key_left = key_up = false;

			// colisão a direita
			point_right.push_back(Vect(31,6));
			point_right.push_back(Vect(31,21));
			point_right.push_back(Vect(31,42));
			// colisão a esquerda
			point_left.push_back(Vect(15,6));
			point_right.push_back(Vect(15,21));
			point_left.push_back(Vect(15,42));
			// colisão abaixo
			point_down.push_back(Vect(15,42));
			point_down.push_back(Vect(31,42));
			// colisão acima
			point_up.push_back(Vect(15,6));
			point_up.push_back(Vect(31,6));

			gravity = 3;
		}

		~Tux (  )
		{
			anim[0]->destroy_textures();
			anim[2]->destroy_textures();
			for (unsigned int i = 0; i < anim.size(); i++)
				delete anim[i];
		}

		void input ( SDL_Event & event )
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_RIGHT:
								key_right = true;
								break;
						case SDLK_LEFT:
								key_left = true;
								break;
						case SDLK_UP:
								key_up = true;
								break;

						default:
								break;
					}
					break;

				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
						case SDLK_RIGHT:
								key_right = false;
								break;
						case SDLK_LEFT:
								key_left = false;
								break;
						case SDLK_UP:
								key_up = false;
								break;

						default:
								break;
					}
					break;
			}
		}

	bool has_coll_tile ( int t )
	{
		for (unsigned int i = 0; i < coll_tiles.size(); i++)
			if (coll_tiles[i] == t)
				return true;
		
		return false;
	}

	bool ground (  )
	{
		return has_coll_tile(map->get_tile(pos.x + point_down[0].x, pos.y + point_down[0].y + 1)) || has_coll_tile(map->get_tile(pos.x + point_down[1].x, pos.y + point_down[1].y + 1));
	}

	int update (  )
	{
		switch (get_state())
		{
			case STANDING:
				if (key_left)
				{
					vel.x = -10;
					dir = LEFT_DIR;
					curr_anim = anim[4];
					curr_anim->reset();
					set_state(WALKING);
					break;
				}
				
				if (key_right)
				{
					vel.x = 10;
					dir = RIGHT_DIR;
					curr_anim = anim[1];
					curr_anim->reset();
					set_state(WALKING);
					break;
				}
				
				if (ground() && key_up)
				{
					vel.y = -20;
					pos.y += vel.y;
					collision_ver(*map, coll_tiles, pos, point_up, vel);
					
					if (dir == LEFT_DIR)
						curr_anim = anim[5];
					else
						curr_anim = anim[2];
					
					curr_anim->reset();
					set_state(JUMPING);
					break;
				}
				
				vel.y += gravity;
				if (vel.y > 20)
					vel.y = 20;
				pos.y += vel.y;
				collision_ver(*map, coll_tiles, pos, point_down, vel);
				
				curr_anim->update();
				break;

			case WALKING:
					if (!ground())
					{
						vel.y = 0;
						if (dir == LEFT_DIR)
							curr_anim = anim[5]; // para esquerda
						else
							curr_anim = anim[2]; // para direita;
						
						curr_anim->reset();
						set_state(JUMPING);
						break;
					}
					else
					{
						if (key_right)
						{
							vel.x = 10;
							dir = RIGHT_DIR;
							curr_anim = anim[1];
						}
						else if (key_left)
						{
							vel.x = -10;
							dir = LEFT_DIR;
							curr_anim = anim[4];
						}
						else
						{
							vel.y = 0;
							vel.x = 0;
							if (dir == LEFT_DIR)
								curr_anim = anim[5];
							else
								curr_anim = anim[2];
							curr_anim->reset();
							set_state(JUMPING);
							break;
						}

						if (key_up)
						{
							vel.y = -20;
							pos.y += vel.y;
							collision_ver(*map, coll_tiles, pos, point_up, vel);
				
							if (dir == LEFT_DIR)
								curr_anim = anim[5];
							else
								curr_anim = anim[2];
				
							curr_anim->reset();
							set_state(JUMPING);
							break;
						}
					}
					
					pos.x += vel.x;
					
					tile_collision(*map, coll_tiles, pos, point_right, vel, MOVE_TO_LEFT);
					tile_collision(*map, coll_tiles, pos, point_left, vel, MOVE_TO_RIGHT);
					
					//collision_hor(*map, coll_tiles, pos, point_left, vel);
					//collision_hor(*map, coll_tiles, pos, point_right, vel);
					curr_anim->update();
					break;

			case JUMPING:
					if (key_right)
					{
						vel.x = 10;
						dir = RIGHT_DIR;
						curr_anim = anim[2];
					}
					else if (key_left)
					{
						vel.x = -10;
						dir = LEFT_DIR;
						curr_anim = anim[5];
					}
					else
					{
						vel.x = 0;
					}
					
					if (ground())
					{
						collision_ver(*map, coll_tiles, pos, point_down, vel);
						vel.y = 0;
						vel.x = 0;
						if (dir == LEFT_DIR)
							curr_anim = anim[3];
						else
							curr_anim = anim[0];
						
						curr_anim->reset();
						set_state(STANDING);
						break;
					}
					
					pos.x += vel.x;
					collision_hor(*map, coll_tiles, pos, point_right, vel);
					collision_hor(*map, coll_tiles, pos, point_left, vel);
					
					vel.y += gravity;
					if (vel.y > 20)
						vel.y = 20;
					pos.y += vel.y;
					
					collision_ver(*map, coll_tiles, pos, point_up, vel);
					collision_ver(*map, coll_tiles, pos, point_down, vel);
					
					curr_anim->update();
					break;
		
			default:
				set_state(STANDING);
				break;
		}
		
		return get_state();
	}

	void draw(Camera *cam,SDL_Renderer *renderer)
	{
		if (curr_anim)
			curr_anim->draw(renderer, cam, pos.x, pos.y);
	}
};


int main (  )
{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Window *window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,20*32,15*32,0);
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_Event event;

		TileMapView map(32);
		//char buf1[] = {"map.txt"};
		//char buf2[] = {"tiles.png"};
		//char buf3[] = {"back.jpg"};
		map.read("map.txt");
		
		Tux player(renderer, &map);

		map.texture = IMG_LoadTexture(renderer,"tiles.png");
		map.remove_tile('.');
		map.set_source('a', (SDL_Rect){32,0,32,32});

		Background background;
		
		background.set_texture(IMG_LoadTexture(renderer, "back.jpg"));
		Camera cam((SDL_Rect){0,0,20*32,15*32}, map.get_dimension());
		
		Texturer::instance()->add_texture(renderer, "background.png");
		

		int done = 0;
		player.set_pos(Vect(0, cam.get_dimension().h/2));

		while (!done)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					done = 1;
				else if (event.type == SDL_KEYDOWN)
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = 1;

				player.input(event);
			}
		
		player.update();
		cam.lookat(player.get_pos());
		
		background.draw(&cam,renderer);
		map.draw(&cam,renderer);
		player.draw(renderer, &cam);
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	CTextureManager::instance()->destroy();
	
	SDL_Quit();
	
	return 0;
}
