
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
	SDL_Color colorB = {0,255,255,255};
	SDL_Color colorIntersect = {255,0,0,255};
	
	SDL_Rect A = {(640 - 100)/2, (480 - 100)/2, 200, 200};
	SDL_Rect B = {0, 0, 300, 300};
	
	Vect pos;
	
	int done = 0;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
			
			if (event.type == SDL_MOUSEMOTION)
			{
				pos.x = event.motion.x;
				pos.y = event.motion.y;
			}
			
			if (event.type == SDL_KEYUP)
			{
				printf("B.x = %d, B.y = %d, A.y + A.h = %d\n", B.x, B.y, A.y + A.h);
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0,0,0,0);
		SDL_RenderClear(renderer);

		// rect A
		fill_rect(renderer, colorA, A);
		// rect B
		B.x = pos.x;
		B.y = pos.y;
		fill_rect(renderer, colorB, B);
		// intersecção A com B
		fill_rect(renderer, colorIntersect, rectIntersect(A,B));
		
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


