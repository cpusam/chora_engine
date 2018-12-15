#include "InfoFile.hpp"

int main(){
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0){
		printf("Erro %s\n", SDL_GetError());
		return 1;
	}
		
	SDL_Window *window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1024,600, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;
	
	
	
	std::string selection = "tiles.png";
	
	InfoFile::instance()->add("mouse.png",renderer);
	InfoFile::instance()->add(selection,renderer);
	
	
	try{
		//Texturer::instance()->add(renderer,"mouse.png");
		
		//Texturer::instance()->add(renderer,"tiles.png");
		
	}catch (const char * e){
		printf("Error %s\n", e);
	}
	bool isRunning = true;
	
	while(isRunning){
		while (SDL_PollEvent(&event)){
			switch (event.type){
			
				case SDL_QUIT:
					isRunning = false;
				break;
				
				case SDL_KEYDOWN:
				
					switch(event.key.keysym.sym){
						
						case SDLK_ESCAPE:
							isRunning = false;
						break;
						
						case SDLK_1:
							selection = "tiles.png";
						break;

						case SDLK_2:
							selection = "tux_left.png";
						break;

						case SDLK_3:
							selection = "tux_right.png";
						break;

						default:
						break;
				}
				
				case SDL_MOUSEMOTION:{

					std::string x = selection;
					x += std::to_string(InfoFile::instance()->getSize());

					
					SDL_Rect aux = {event.motion.x - (InfoFile::instance()->getPos(x)->w/2),
					event.motion.y - (InfoFile::instance()->getPos(x)->w/2),0,0};
					
					
					InfoFile::instance()->setPos(x,aux);
					
					InfoFile::instance()->setPos("mouse.png1",aux);
					
				}break;
				
				
				case SDL_MOUSEBUTTONDOWN:{
					
					std::string x = selection;
					x += std::to_string(InfoFile::instance()->getSize());
					
					SDL_Rect aux = {event.button.x - (InfoFile::instance()->getPos(x)->w/2),
					event.button.y - (InfoFile::instance()->getPos(x)->w/2),0,0};
					
					InfoFile::instance()->add(selection,renderer);
					
					InfoFile::instance()->setPos(x,aux);
					
					std::cout << x << std::endl;
					
				}
				break;
			}
		}
		
		FPSManager::instance()->update();
		if (FPSManager::instance()->getDelta() > 0){
			
			SDL_SetRenderDrawColor(renderer, 255,255,255,255);
			SDL_RenderClear(renderer);
			
			InfoFile::instance()->drawAll(renderer);
			
			SDL_RenderCopy(
				renderer,InfoFile::instance()->getTexture("mouse.png1"),NULL,
				InfoFile::instance()->getPos("mouse.png1")
			);
			
			SDL_RenderPresent(renderer);
		}		
	}
	
	return 0;
}
