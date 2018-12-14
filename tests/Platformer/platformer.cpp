#include "Chora.hpp"

#if defined(CHORA_WRITER_HPP)
	#warning "writer.hpp definido"
#endif

#include <iostream>
#include <vector>
#include <iterator>

#warning "precisa atualizar este programa"

using namespace std;

enum EPlayerState
{
	FLYING,
	STANDING,
	WALKING,
	JUMPING,
};

enum EPlayerDir
{
	LEFT_DIR,
	RIGHT_DIR,
};

float toAcc ( float p )
{
	float vel = (FPSManager::instance()->get_delta() / 1000.f);
	
	if (vel == 0)
		vel = 1;
	
	return p / vel;
}

class Tux: public Entity {
		bool key_right, key_left, key_up, key_down;
		vector <Vect> point_up;
		vector <Vect> point_down;
		vector <Vect> point_left;
		vector <Vect> point_right;
		vector <int> coll_tiles;
		float gravity;
		float velStartJump;

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
			int fd = 150;
			// animações de direita
			anim.push_back(new Animation());
			anim[0]->addFrame(texture,(SDL_Rect){144,0,48,43}, 60);
			anim.push_back(new Animation());
			anim[1]->addFrame(texture, (SDL_Rect){0 ,0,48,43}, fd);
			anim[1]->addFrame(texture, (SDL_Rect){48,0,48,43}, fd);
			anim[1]->addFrame(texture, (SDL_Rect){96,0,48,43}, fd);
			anim[1]->addFrame(texture, (SDL_Rect){48,0,48,43}, fd);
			anim.push_back(new Animation());
			anim[2]->addFrame(texture, (SDL_Rect){192,0,48,43}, fd);

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
			anim[3]->addFrame(texture,(SDL_Rect){144,0,48,43}, 60);
			anim.push_back(new Animation());
			anim[4]->addFrame(texture, (SDL_Rect){0 ,0,48,43}, fd);
			anim[4]->addFrame(texture, (SDL_Rect){48,0,48,43}, fd);
			anim[4]->addFrame(texture, (SDL_Rect){96,0,48,43}, fd);
			anim[4]->addFrame(texture, (SDL_Rect){48,0,48,43}, fd);
			anim.push_back(new Animation());
			anim[5]->addFrame(texture, (SDL_Rect){192,0,48,43}, fd);
			
			
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

			gravity = (90);
			acc.x = (100);
			acc.y = (100);
			velStartJump = (200);
			setState(STANDING);
		}

		~Tux (  )
		{
			anim[0]->destroyTextures();
			anim[2]->destroyTextures();
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
						
						case SDLK_DOWN:
								key_down = true;
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
						case SDLK_DOWN:
								key_down = false;
								break;

						default:
								break;
					}
					break;
			}
		}

		void input ( Widget & widget )
		{
			
			if (widget.getChild("keyLeft")->getState() == 3) // pressionado
				key_left = true;
			else
				key_left = false;
			
			if (widget.getChild("keyRight")->getState() == 3) // pressionado
				key_right = true;
			else
				key_right = false;
			
			if (widget.getChild("keyJump")->getState() == 3) // pressionado
				key_up = true;
			else
				key_up = false;
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
		return has_coll_tile(map->get_tile(position.x + point_down[0].x, position.y + point_down[0].y + 1)) || has_coll_tile(map->get_tile(position.x + point_down[1].x, position.y + point_down[1].y + 1));
	}

	int update (  )
	{
		switch (getState())
		{
			case FLYING:
				if (key_left)
				{
					vel.x = -2;
				}
				else if (key_right)
				{
					vel.x = 2;
				}
				else
				{
					vel.x = 0;
				}
				
				if (key_up)
				{
					vel.y = -2;
				}
				else if (key_down)
				{
					vel.y = 2;
				}
				else
				{
					vel.y = 0;
				}
				position += vel;
				curr_anim = anim[1];
				curr_anim->update();
				break;
				
			case STANDING:
				if (key_left)
				{
					vel.x = -acc.x;
					dir = LEFT_DIR;
					curr_anim = anim[4];
					curr_anim->reset();
					setState(WALKING);
					break;
				}
				
				if (key_right)
				{
					vel.x = acc.x;
					dir = RIGHT_DIR;
					curr_anim = anim[1];
					curr_anim->reset();
					setState(WALKING);
					break;
				}
				
				if (ground() && key_up)
				{
					vel.y = -velStartJump;
					position.y += vel.y * FPSManager::instance()->get_delta()/1000.f;
					collisionVertical(*map, coll_tiles, position, point_up, vel);
					
					if (dir == LEFT_DIR)
						curr_anim = anim[5];
					else
						curr_anim = anim[2];
					
					curr_anim->reset();
					setState(JUMPING);
					break;
				}
				
				vel.y += gravity;
				if (vel.y > velStartJump)
					vel.y = velStartJump;
				position.y += vel.y * FPSManager::instance()->get_delta()/1000.f;
				collisionVertical(*map, coll_tiles, position, point_down, vel);
				
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
						setState(JUMPING);
						break;
					}
					else
					{
						if (key_right)
						{
							vel.x = acc.x;
							dir = RIGHT_DIR;
							curr_anim = anim[1];
						}
						else if (key_left)
						{
							vel.x = -acc.x;
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
							setState(JUMPING);
							break;
						}

						if (key_up)
						{
							vel.y = -velStartJump;
							position.y += vel.y * FPSManager::instance()->get_delta()/1000.f;
							collisionVertical(*map, coll_tiles, position, point_up, vel);
				
							if (dir == LEFT_DIR)
								curr_anim = anim[5];
							else
								curr_anim = anim[2];
				
							curr_anim->reset();
							setState(JUMPING);
							break;
						}
					}
					
					position.x += vel.x * FPSManager::instance()->get_delta()/1000.f;
					
					//tile_collision(*map, coll_tiles, position, point_right, vel, MOVE_TO_LEFT);
					//tile_collision(*map, coll_tiles, position, point_left, vel, MOVE_TO_RIGHT);
					
					//collisionHorizontal(*map, coll_tiles, position, point_left, vel);
					//collisionHorizontal(*map, coll_tiles, position, point_right, vel);
					curr_anim->update();
					break;

			case JUMPING:
					if (key_right)
					{
						vel.x = acc.x;
						dir = RIGHT_DIR;
						curr_anim = anim[2];
					}
					else if (key_left)
					{
						vel.x = -acc.x;
						dir = LEFT_DIR;
						curr_anim = anim[5];
					}
					else
					{
						vel.x = 0;
					}
					
					if (ground())
					{
						collisionVertical(*map, coll_tiles, position, point_down, vel);
						vel.y = 0;
						vel.x = 0;
						if (dir == LEFT_DIR)
							curr_anim = anim[3];
						else
							curr_anim = anim[0];
						
						curr_anim->reset();
						setState(STANDING);
						break;
					}
					
					position.x += vel.x * FPSManager::instance()->get_delta()/1000.f;
					collisionHorizontal(*map, coll_tiles, position, point_right, vel);
					collisionHorizontal(*map, coll_tiles, position, point_left, vel);
					
					vel.y += gravity;
					if (vel.y > velStartJump)
						vel.y = velStartJump;
					position.y += vel.y * FPSManager::instance()->get_delta()/1000.f;
					
					collisionVertical(*map, coll_tiles, position, point_up, vel);
					collisionVertical(*map, coll_tiles, position, point_down, vel);
					
					curr_anim->update();
					break;
		
			default:
				setState(STANDING);
				break;
		}
		
		return getState();
	}

	void draw(SDL_Renderer *renderer, Camera *cam)
	{
		if (curr_anim)
			curr_anim->draw(renderer, cam, position.x, position.y);
	}
};


int main (  )
{
		SDL_Init(SDL_INIT_VIDEO);
		
		if (TTF_Init() < 0)
		{
			printf("Erro %s\n", SDL_GetError());
			return 1;
		}
		
		SDL_Window *window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,20*32,15*32,SDL_WINDOW_SHOWN);
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
		map.setSourceRect('a', (SDL_Rect){32,0,32,32});

		Background background;
		
		background.setTexture(IMG_LoadTexture(renderer, "back.jpg"));
		Camera cam((SDL_Rect){100,100,15*32,10*32}, map.getDimension());
		
		Texturer::instance()->add(renderer, "background.png");
		Widget widget;
		GuiButton * button;
		try
		{
			Writer::instance()->setRenderer(renderer);
			Writer::instance()->loadFont("04B_20__.TTF","04B_20", 50);
			
			button = new GuiButton((SDL_Rect){20,12*32, 64,32}, "<-");
			button->setID("keyLeft");
			widget.addChild(button);
		
			button = new GuiButton((SDL_Rect){4*32 + 20,12*32, 64,32}, "->");
			button->setID("keyRight");
			widget.addChild(button);	

			button = new GuiButton((SDL_Rect){18*32,12*32, 64,32}, "^");
			button->setID("keyJump");
			widget.addChild(button);
			button = 0;
		} catch (const char * e)
		{
			printf("Error %s\n", e);
		}

		
		

		int done = 0;
		player.setPosition(Vect(0, cam.getDimension().h/2));

		while (!done)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					done = 1;
				else if (event.type == SDL_KEYDOWN)
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = 1;
				
				widget.input(event);
				player.input(event);
			}
		
		//player.input(widget);
		
		player.update();
		widget.update();
		cam.lookat(player.getPosition());
		
		FPSManager::instance()->update();
		if (FPSManager::instance()->get_delta() > 0)
		{
			SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			SDL_RenderClear(renderer);
			background.draw(renderer, &cam);
			map.draw(renderer,&cam);
			player.draw(renderer, &cam);
			widget.draw(renderer);
			SDL_RenderPresent(renderer);
		}
		//SDL_Delay(60);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Texturer::instance()->destroy();
	
	SDL_Quit();
	
	return 0;
}
