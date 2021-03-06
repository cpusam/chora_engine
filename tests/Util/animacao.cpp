
#include "Chora.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main ( int argc, char **argv )
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Erro ao iniciar SDL\n");
		return 1;
	}
	
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	
	
	window = SDL_CreateWindow("Util test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	if (!window)
	{
		printf("Erro ao criar janela\n");
		return 1;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Erro ao criar renderer\n");
		return 1;
	}
	
	SDL_Color colorA = {255,255,0,255};
	//SDL_Color colorB = {0,255,255,255};
	
	Vect position;
	Vect velocity;
	SDL_Rect limit = {0,0,640,480}, rectB = {0,0,200,200};
	Camera camera((SDL_Rect){0,0,300,200}, limit);
	Animation anim;
	SDL_Texture * texture = Texturer::add(renderer,"image.png");
	int w, h;
	w = textureWidth(texture);
	h = textureHeight(texture);
	anim.addFrame(texture, (SDL_Rect){0,0,w,h}, rectB, 60);
	
	int done = 0;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
			
			if (event.type == SDL_MOUSEMOTION)
			{
				position.x = event.motion.x - rectB.w/2;
				position.y = event.motion.y - rectB.h/2;
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						velocity.y = -5;
						break;
					case SDLK_DOWN:
						velocity.y = 5;
						break;
					case SDLK_RIGHT:
						velocity.x = 5;
						break;
					case SDLK_LEFT:
						velocity.x = -5;
						break;
					default:
						break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						velocity.y = 0;
						break;
					case SDLK_DOWN:
						velocity.y = 0;
						break;
					case SDLK_RIGHT:
						velocity.x = 0;
						break;
					case SDLK_LEFT:
						velocity.x = 0;
						break;
					default:
						break;
				}
			}
			
		}
		
		position.x += velocity.x;
		position.y += velocity.y;
		
		SDL_SetRenderDrawColor(renderer, 0,0,0,0);
		SDL_RenderClear(renderer);

		SDL_Rect camRect = camera.getDimension();
		fillRect(renderer, colorA, camRect);
		
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	Texturer::destroy();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


