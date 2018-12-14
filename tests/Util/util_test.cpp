
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
	
	Texturer::instance()->add(renderer, "image.png");
	SDL_Texture * image = Texturer::instance()->get("image.png");
	Animation anim;
	
	//anim.

	
	Camera cam((SDL_Rect){40,40,200,200}, (SDL_Rect){0,0,2000,200});
	SDL_Color color = {255, 255, 0, 255};
	SDL_Color bg_color = {255, 255, 255, 255};
	
	Vect position, vel;
	
	int done = 0;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
			
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				color.r = rand() % 256;
				color.g = rand() % 256;
				color.b = rand() % 256;
				color.a = 255;
			}
			
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_Rect d = {event.button.x,event.button.y, cam.getDimension().w/2,cam.getDimension().h/2};
				d.x = d.x - d.w;
				d.y = d.y - d.h;
				
				cam.setScreenPosition(d.x, d.y);
			}
			
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_RIGHT:
						vel.x = 5;
						break;
					case SDLK_LEFT:
						vel.x = -5;
						break;
					case SDLK_UP:
						vel.y = -5;
						break;
					case SDLK_DOWN:
						vel.y = 5;
						break;
					
					default:
						break;
				}
			}
			
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_RIGHT:
					case SDLK_LEFT:
						vel.x = 0;
						break;
					case SDLK_UP:
					case SDLK_DOWN:
						vel.y = 0;
						break;
					
					default:
						break;
				}
			}
		}
		if (vel.x || vel.y)
			printf("Está movendo %u\n", SDL_GetTicks());
		position += vel;
		cam.lookat(position);
		
		SDL_SetRenderDrawColor(renderer, 0,0,0,0);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
		SDL_Rect r = cam.getDimension();
		SDL_RenderFillRect(renderer, &r);
		// lado esquerdo
		fillRect(renderer, &cam, color, (SDL_Rect){-20, 100, 100, 20});
		// lado direito
		fillRect(renderer, &cam, color, (SDL_Rect){position.x, position.y, 100, 20});
		// lado de cima
		fillRect(renderer, &cam, color, (SDL_Rect){100, -20, 20, 100});
		// lado de baixo
		fillRect(renderer, &cam, color, (SDL_Rect){100, 180, 20, 100});
		
		int ret = 0;
		if ((ret = drawTexture(renderer, &cam, Texturer::instance()->get("image.png"), 50, 50)) < 0)
			printf("Retornou %d em %u\n", ret, SDL_GetTicks());
		
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


