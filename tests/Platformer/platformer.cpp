#include "Chora.hpp"

#include <iostream>
#include <vector>
#include <memory>


class Tux: public Entity 
{
	bool key_right, key_left, key_up, key_down;
	float gravity;
	float velStartJump;
	Vect configAcc;
	std::map<std::string, Input *> controls;
	int inputType;

	public:
		enum EPlayerState
		{
			FLYING,
			STANDING,
			WALKING,
			JUMPING,
		};
	public:
		Tux ( TileMap * m )
		{
			SDL_Texture *texture = Texturer::get("tuxSprites");
			if (!texture)
				throw Exception("Error:: textura tuxSprites não encontrada");
			setLevel(m);

			int frameDelay = 150;
			// animações de direita
			{
				Animation aux;
				aux.addFrame(texture,(SDL_Rect){144,0,48,43}, 60);
				anim["stand"] = aux;
				aux.clearFrames();

				aux.addFrame(texture, (SDL_Rect){0 ,0,48,43}, frameDelay);
				aux.addFrame(texture, (SDL_Rect){48,0,48,43}, frameDelay);
				aux.addFrame(texture, (SDL_Rect){96,0,48,43}, frameDelay);
				aux.addFrame(texture, (SDL_Rect){48,0,48,43}, frameDelay);
				anim["walk"] = aux;
				aux.clearFrames();

				aux.addFrame(texture, (SDL_Rect){192,0,48,43}, frameDelay);
				anim["jump"] = aux;
			}

			//adiciona sólidopara colisão com o mapa
			addSolid('a');

			changeAnim("stand", true);
			setDir(Direction::RIGHT_DIR);
			key_right = key_left = key_up = false;

			// colisão
			setCollRect((SDL_Rect){15,6,16,36}, 5);

			gravity = 90;
			velStartJump = 200;
			maxVel.set(15,20);
			setDamping(Vect(1.0,0));
			setState(STANDING);
			configAcc.set(100,100);

			controls["left"] = new Key(SDLK_LEFT);
			controls["right"] = new Key(SDLK_RIGHT);
			controls["down"] = new Key(SDLK_DOWN);
			controls["up"] = new Key(SDLK_UP);
			controls["jump"] = new Key(SDLK_s);
			
			inputType = 0;
			#ifdef __ANDROID__
				inputType = 1;
			#endif
		}

		~Tux (  )
		{
			for (auto & it: controls)
				delete it.second;
		}

		void input ( SDL_Event & event ) override
		{
			for (auto & it: controls)
				it.second->input(event);
		}

		void guiInput ( Widget & widget )
		{
			
			if (widget.getChild("keyLeft")->getState() == GuiButton::PRESSED) // pressionado
				key_left = true;
			else
				key_left = false;
			
			if (widget.getChild("keyRight")->getState() == GuiButton::PRESSED) // pressionado
				key_right = true;
			else
				key_right = false;
			
			if (widget.getChild("keyJump")->getState() == GuiButton::PRESSED) // pressionado
				key_up = true;
			else
				key_up = false;
		}

	void updateControls (  )
	{
		if (inputType == 1)
			return;
		
		for (auto & it: controls)
			it.second->update();
		
		key_left = controls["left"]->getState() == Key::HOLD;
		key_right = controls["right"]->getState() == Key::HOLD;
		key_down = controls["down"]->getState() == Key::HOLD;
		key_up = controls["up"]->getState() == Key::PRESS;
	}

	bool isGround (  ) override
	{
		SDL_Rect rect = getCollRect();
		return isSolid(Vect(rect.x, rect.y + rect.h + 1)) || isSolid(Vect(rect.x + rect.w, rect.y + rect.h + 1));
	}

	bool collisionHorizontal (  )
	{
		if (collisionX())
		{
			acc.x = 0;
			velocity.x = 0;
			return true;
		}

		SDL_Rect dim = level->getDimension();
		if (position.x + collRect.x < dim.x)
		{
			position.x = dim.x - collRect.x;
			velocity.x = 0;
			acc.x = 0;
			return true;
		}
		else if (position.x + collRect.x + collRect.w > dim.x + dim.w)
		{
			position.x = dim.x + dim.w - (collRect.x + collRect.w);
			velocity.x = 0;
			acc.x = 0;
			return true;
		}

		return false;
	}

	bool collisionVertical (  )
	{
		if (collisionY())
		{
			acc.y = 0;
			velocity.y = 0;
			return true;
		}
		return false;
	}

	int update (  )
	{
		updateControls();
		switch (getState())
		{
			case FLYING:
				if (key_left)
				{
					acc.x = -configAcc.x;
				}
				else if (key_right)
				{
					acc.x = configAcc.x;
				}
				else
				{
					acc.x = 0;
				}
				
				if (key_up)
				{
					acc.y = -configAcc.y;
				}
				else if (key_down)
				{
					acc.y = configAcc.y;
				}
				else
				{
					acc.y = 0;
				}
				moveX();
				moveY();
				
				updateAnim();
				break;
				
			case STANDING:
				if (key_left)
				{
					acc.x = -configAcc.x;
					changeDir(Direction::LEFT_DIR);
					changeAnim("walk", true);
					setState(WALKING);
					break;
				}
				
				if (key_right)
				{
					acc.x = configAcc.x;
					changeDir(Direction::RIGHT_DIR);
					changeAnim("walk", true);
					setState(WALKING);
					break;
				}
				
				if (isGround() && key_up)
				{
					velocity.y = -velStartJump;
					position.y += velocity.y * FPSManager::instance()->getDelta()/1000.f;
					collisionVertical();
					changeAnim("jump", true);
					setState(JUMPING);
					break;
				}
				
				velocity.y += gravity;
				if (velocity.y > velStartJump)
					velocity.y = velStartJump;
				position.y += velocity.y * FPSManager::instance()->getDelta()/1000.f;
				collisionVertical();
				
				updateAnim();
				break;

			case WALKING:
					if (!isGround())
					{
						velocity.y = 0;
						changeAnim("jump", true);
						setState(JUMPING);
						break;
					}
					else
					{
						if (key_right)
						{
							acc.x = configAcc.x;
							changeDir(Direction::RIGHT_DIR);
						}
						else if (key_left)
						{
							acc.x = -configAcc.x;
							changeDir(Direction::LEFT_DIR);
						}
						else
						{
							velocity.x = 0;
							acc.x = 0;
							changeAnim("stand",true);
							setState(STANDING);
							break;
						}

						if (key_up)
						{
							velocity.y = -velStartJump;
							position.y += velocity.y * FPSManager::instance()->getDelta()/1000.f;
							collisionVertical();
							changeAnim("jump", true);
							setState(JUMPING);
							break;
						}
					}
					
					moveX();
					collisionHorizontal();
					
					updateAnim();
					break;

			case JUMPING:
					if (key_right)
					{
						acc.x = configAcc.x;
						changeDir(Direction::RIGHT_DIR);
					}
					else if (key_left)
					{
						acc.x = -configAcc.x;
						changeDir(Direction::LEFT_DIR);
					}
					else
					{
						acc.x = 0;
					}
					
					if (isGround())
					{
						collisionVertical();
						velocity.y = 0;
						velocity.x = 0;
						changeAnim("stand", true);
						setState(STANDING);
						break;
					}
					
					moveX();
					collisionHorizontal();
					
					moveY(gravity);
					collisionVertical();
					
					updateAnim();
					break;
		
			default:
				setState(STANDING);
				break;
		}
		
		return getState();
	}
};


int main (  )
{
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init((unsigned int)-1);

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
		
		Texturer::add(IMG_LoadTexture(renderer, "tux.png"), "tuxSprites");

		Tux player(&map);

		map.texture = IMG_LoadTexture(renderer,"tiles.png");
		map.removeTile('.');
		map.setSourceRect('a', (SDL_Rect){32,0,32,32});

		Background background;
		
		background.setTexture(IMG_LoadTexture(renderer, "back.jpg"));
		Camera cam((SDL_Rect){0,0,20*32,15*32}, map.getDimension());
		SDL_RenderSetLogicalSize(renderer, 20*32,15*32);

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
				player.guiInput(widget);
			}
		
		#ifdef __ANDROID__
		player.guiInput(widget);
		#endif
		
		
		widget.update();
		player.update();
		cam.lookat(player.getPosition());
		
		FPSManager::instance()->update();
		if (FPSManager::instance()->getDelta() > 0)
		{
			//SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			//SDL_RenderClear(renderer);
			background.draw(renderer, &cam);
			map.draw(renderer,&cam);
			player.draw(renderer, &cam);
			#ifdef __ANDROID__
			widget.draw(renderer);
			#endif
			SDL_RenderPresent(renderer);
		}
		//SDL_Delay(60);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Texturer::instance()->destroy();
	
	std::vector<Widget *> childs = widget.getChildren();
	for (Widget * child: childs)
		delete child;
	widget.clearChildren();

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}
