#include <iostream>
#include "Chora.hpp"


#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT  480


int main ( int argc, char** argv )
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL_Init Erro: "<<SDL_GetError()<<std::endl;
		return 1;
	}

	if (TTF_Init() < 0)
	{
		std::cerr<<"TTF_Init Erro: "<<TTF_GetError()<<std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Window * window = SDL_CreateWindow("Test Label", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
	                                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cerr<<"SDL_CreateWindow Erro: "<<SDL_GetError()<<std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_RendererInfo drinfo;
	SDL_GetRenderDriverInfo (0, &drinfo);
	Uint32 renderFlags = 0;

	if (drinfo.flags & SDL_RENDERER_ACCELERATED)
		renderFlags = renderFlags | SDL_RENDERER_ACCELERATED;
	else if (drinfo.flags & SDL_RENDERER_SOFTWARE)
		renderFlags = renderFlags | SDL_RENDERER_SOFTWARE;

	if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
		renderFlags = renderFlags | SDL_RENDERER_TARGETTEXTURE;

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, renderFlags);
	if (renderer == nullptr)
	{
		std::cerr<<"SDL_CreateRenderer Erro: "<<SDL_GetError()<<std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Texture * targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (targetTexture == nullptr)
	{
		std::cerr<<"Erro: "<<SDL_GetError()<<std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	Writer::instance()->setRenderer(renderer);
	try{
		Writer::instance()->loadFont("./fonts/inhouseedition.ttf", "default", 80);
	}catch(Exception & e){
		e.what();
	}
	
	GuiLabel * label = new GuiLabel("Chora Engine", (SDL_Color){0,255,0,128}, "default");
	label->setPosition(Vect((SCREEN_WIDTH - textureWidth(label->getTexture()))/2, (SCREEN_HEIGHT - textureHeight(label->getTexture()))/2));
	label->show(true);
	
	SDL_Event event;
	Key fullscreen(SDLK_f), print(SDLK_p);

	FPSManager::instance()->set_framerate(60); // 60 fps

	Uint32 fullscreenFlags = 0;
	bool done = false;
	std::cout<<"Aperte f para fullscreen  P para imprimir informação\n";
	while (done == false)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = true;
			
			fullscreen.input(event);
			print.input(event);
		}

		fullscreen.update();
		print.update();

		if (print.getState() == Key::PRESS)
		{
			std::cout<<"label.textura = " << std::endl
			<< "w = " << textureWidth(label->getTexture()) << std::endl
			<< "h = " << textureHeight(label->getTexture()) <<std::endl
			<< "X/Y = "<<label->getPosition().x << "/"<<label->getPosition().y<<std::endl;
		}

		if (fullscreen.getState() == Key::PRESS)
		{
			fullscreenFlags = (fullscreenFlags ^ SDL_WINDOW_FULLSCREEN);
			SDL_SetWindowFullscreen(window, fullscreenFlags);
		}

		if (FPSManager::instance()->get_delta() > 0)
		{
			SDL_SetRenderTarget(renderer, targetTexture);
			SDL_SetRenderDrawColor(renderer, 0,0,0,255);
			SDL_RenderClear(renderer);

			label->draw(renderer);

			SDL_SetRenderTarget(renderer, nullptr);
			SDL_RenderCopyEx(renderer, targetTexture, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE);
			SDL_RenderPresent(renderer);
		}

		FPSManager::instance()->update();
	}

	Writer::destroy();
	FPSManager::destroy();

	SDL_DestroyTexture(targetTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
