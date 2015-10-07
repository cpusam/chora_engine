#include "texturer.hpp"

Texturer * Texturer::singleton = 0;

Texturer::Texturer(){}

Texturer::~Texturer(){}

Texturer* Texturer::instance(){
	if( singleton == 0 ){
		singleton = new Texturer();
	}
	return singleton;
}

void Texturer::add (SDL_Renderer * renderer, std::string path){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->path == path){
			rem(textureID[i]->name);
			textureID.push_back(new TextureID(path,renderer));
			return;
		}
	}

	textureID.push_back(new TextureID(path,renderer));
}

void Texturer::add (SDL_Texture *tex, std::string name){
	for(unsigned int i = 0;i<textureID.size();i++){
		if(textureID[i]->texture == tex){
			
			rem(textureID[i]->name);
			textureID.push_back(new TextureID(tex, name));
			return;
		}
	}
	
	textureID.push_back(new TextureID(tex, name));
}

void Texturer::rem (std::string name){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
			textureID[i]->destroy();
			textureID.erase(textureID.begin() + i);
			break;
		}
	}
}

SDL_Texture *Texturer::get_texture(std::string name) {
	//std::cout << name << "\n";
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
				return textureID[i]->texture;
		}

	}

	std::cout << "[Texture Manager] Error : Texture \"" << name << "\" não encontrado!\n" << std::endl;
	throw "[TextureManager] Error : Texture not found!";
}

void Texturer::destroy() {
	for (unsigned int i = 0, end = textureID.size(); i < end; i++)
		delete textureID[i];

	textureID.clear();
}

