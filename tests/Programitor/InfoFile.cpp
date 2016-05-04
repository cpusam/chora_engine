#include "InfoFile.hpp"

InfoFile *InfoFile::singleton = nullptr;
		
InfoFile::InfoFile(){
	auxSurface = nullptr;
	auxTexture = nullptr;
}
	
InfoFile::~InfoFile(){
	destroy();
}
	
InfoFile *InfoFile::instance(){
	if(singleton == nullptr){
		singleton = new InfoFile();
	}
	return singleton;
}
	
void InfoFile::add(std::string n, SDL_Renderer *r){
	
	auxSurface = IMG_Load(n.c_str());
	
	if(auxSurface == NULL){
		std::cout << "ERROR IMAGE " << n << '\n';
	}
	
	pos.push_back((SDL_Rect){-1000,-1000,auxSurface->w,auxSurface->h});
	
	auxTexture = SDL_CreateTextureFromSurface(r,auxSurface);
	
	std::string m = n;
	
	m += std::to_string(pos.size());
	
	Texturer::instance()->add(auxTexture, m);
	
	name.push_back(m);
	
}
	
SDL_Texture *InfoFile::getTexture(std::string n){
	return Texturer::instance()->get_texture(n);
}
	
SDL_Rect *InfoFile::getPos(std::string n){
	for (unsigned int i = 0, end = name.size(); i < end; i++){
		if(name[i] == n){
			return &pos[i];
		}

	}		
	return nullptr;
}

void InfoFile::setPos(std::string n, SDL_Rect p){
	for (unsigned int i = 0, end = name.size(); i < end; i++){
		if(name[i] == n){
			pos[i].x = p.x;
			pos[i].y = p.y;
		}
	}		
}

void InfoFile::drawAll(SDL_Renderer *r){
	for(uint i = 0; i<name.size();i++){
		SDL_RenderCopy(r,getTexture(name[i]),NULL,getPos(name[i]));	
		std::cout << name[i] << '\n';
	}
}

void InfoFile::destroy(){
	SDL_FreeSurface(auxSurface);
	SDL_DestroyTexture(auxTexture);		
}

int InfoFile::getSize(){
	return pos.size();
}
