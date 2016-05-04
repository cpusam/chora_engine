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

SDL_Texture * Texturer::addTexture (SDL_Renderer * renderer, std::string path){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->path == path){
			rem(textureID[i]->name);
			textureID.push_back(new TextureID(path,renderer));
			return textureID.back()->texture;
		}
	}

	textureID.push_back(new TextureID(path,renderer));
	return textureID.back()->texture;
}

void Texturer::addTexture (SDL_Texture *tex, std::string name){
	for(unsigned int i = 0;i<textureID.size();i++){
		if(textureID[i]->texture == tex){
			
			rem(textureID[i]->name);
			textureID.push_back(new TextureID(tex, name));
			return;
		}
	}
	
	textureID.push_back(new TextureID(tex, name));
}

void Texturer::remTexture (std::string name){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
			textureID[i]->destroy();
			textureID.erase(textureID.begin() + i);
			break;
		}
	}
}

SDL_Texture *Texturer::getTexture(std::string name) {
	//std::cout << name << "\n";
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
				return textureID[i]->texture;
		}

	}

	std::cout << "[Texture Manager] Error : Texture \"" << name << "\" não encontrado!\n" << std::endl;
	throw "[TextureManager] Error : Texture not found!";
}

void Texturer::destroyAll() {
	for (unsigned int i = 0, end = textureID.size(); i < end; i++)
		delete textureID[i];

	textureID.clear();
}

SDL_Texture * Texturer::add (SDL_Renderer * renderer, std::string path)
{
	return instance()->addTexture(renderer, path);
}

void Texturer::add (SDL_Texture *tex, std::string name )
{
	instance()->addTexture(tex,name);
}

void Texturer::rem ( std::string name )
{
	instance()->remTexture(name);
}

SDL_Texture* Texturer::get ( std::string name )
{
	return instance()->getTexture(name);
}

void Texturer::destroy() {
	instance()->destroyAll();
}

