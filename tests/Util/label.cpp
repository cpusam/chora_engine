#include "../../Chora.hpp"

int main ()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("Label",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	
	if (!window || !renderer)
	{
		printf("Erro %s\n", SDL_GetError());
		return 1;
	}
	
	TTF_Init();
	
	// tem que configurar o writer primeiro
	Writer::instance()->set_renderer(renderer);
	
	GuiLabel * label;
	try
	{
		// e depois tem que carregar a fonte antes de desenhar algum texto
		// pode dar uma exceção nesse ponto, por isso use try catch
		Writer::instance()->set_font("04B_20__.TTF", 20);
	
		label = new GuiLabel("Eu sou uma Label", (SDL_Color){255,0,0,255});
	}
	catch (const char * e)
	{
		printf("Erro %s\n", e);
	}
	label->set_pos(Vect((640 - label->get_texture_width())/2, 480/2));
	
	int done = 0;
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
		}
		
		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderClear(renderer);
		
		label->draw(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	delete label;
	
	SDL_Quit();
	return 0;
}


