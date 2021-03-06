#include "Chora.hpp"

int main (int argc, char ** argv )
{
	if (SDL_Init(SDL_Init(SDL_INIT_VIDEO)) < 0)
	{
		SDL_Log(SDL_GetError(), SDL_LOG_CATEGORY_APPLICATION);
		return 1;
	}

	if (TTF_Init() < 0)
	{
		SDL_Log(TTF_GetError(), SDL_LOG_CATEGORY_APPLICATION);
		SDL_Quit();
		return 1;
	}

	SDL_Window * window = SDL_CreateWindow("Text Input", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window)
	{
		SDL_Log(SDL_GetError(), SDL_LOG_CATEGORY_APPLICATION);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		SDL_Log(SDL_GetError(), SDL_LOG_CATEGORY_APPLICATION);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	Writer::instance()->setRenderer(renderer);
	Writer::instance()->loadFont("fonts/Jumpman.ttf", "=>default", 50);
	FPSManager::instance()->setFramerate(60);

	int done = 0;
	GuiTextInput * textinput = new GuiTextInput(50, (SDL_Color){255, 0, 0, 255}, 10);
	textinput->show();
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = 1;
			
			textinput->input(event);

			/*
			if (event.type == SDL_MOUSEMOTION)
			{
				textinput->setPosition(Vect(event.motion.x, event.motion.y));
			}
			*/
		}

		FPSManager::instance()->update();
		textinput->update();

		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);
		textinput->draw(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}

	Writer::instance()->destroy();
	FPSManager::instance()->destroy();
	TTF_Quit();
	SDL_Quit();

	return 0;
}