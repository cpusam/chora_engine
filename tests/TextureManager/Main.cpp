#include "../../Chora.hpp"

int main(){
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Textures", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	
	if(window == NULL){std::cout << "Error1!\n";return -1;}
	
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL){std::cout << "Error2!\n";return -1;}
	
	if (IMG_Init(IMG_INIT_PNG) < 0) {std::cout << "Error3!\n";return -1;}
		
	
	SDL_Event event;
	
	std::string str("background.png");
	try{
	Texturer::add(renderer, str);
	} catch (const char * e)
	{
		std::cout << "Erro: " << e << std::endl;
	}
	std::cout<<"back = "<<str<<std::endl;
	bool isRunning = true;
	
	while(isRunning){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					isRunning = false;
				break;
			}
		}
		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,Texturer::get(str),NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(60);
	}
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	Texturer::destroy();
	Texturer::instance();
	
	SDL_Quit();
	
	return 0;
}
