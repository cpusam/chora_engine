#include "texturer.hpp"

std::atomic<Texturer *> Texturer::singleton{nullptr};
std::mutex Texturer::myMutex;

Texturer::Texturer(){}

Texturer::~Texturer(){}

Texturer* Texturer::instance(){
	if(!singleton){
		std::lock_guard<std::mutex> lock(myMutex);
		if (!singleton)
			singleton = new Texturer();
	}
	return singleton;
}

SDL_Texture * Texturer::addTexture (SDL_Renderer * renderer, std::string path){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i].path == path){
			//rem(textureID[i]->name);
			//textureID.push_back(new TextureID(path,renderer));
			return textureID.at(i).texture;
		}
	}

	textureID.push_back(TextureID(path,renderer));
	return textureID.back().texture;
}

void Texturer::addTexture (SDL_Texture *tex, std::string name){
	if (tex == nullptr)
		throw IMG_GetError();
	
	for(unsigned int i = 0;i<textureID.size();i++){
		if(textureID[i].texture == tex){
			
			//rem(textureID[i]->name);
			//textureID.push_back(new TextureID(tex, name));
			return;
		}
	}
	
	textureID.push_back(TextureID(tex, name));
}

SDL_Texture * Texturer::addTexture (SDL_Renderer * renderer, std::string path, SDL_Color colorKey){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i].path == path){
			//rem(textureID[i]->name);
			//textureID.push_back(new TextureID(path,renderer));
			return textureID.at(i).texture;
		}
	}

	textureID.push_back(TextureID(path,renderer, colorKey));
	return textureID.back().texture;
}

void Texturer::remTexture (std::string name){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i].name == name){
			textureID[i].destroy();
			textureID.erase(textureID.begin() + i);
			break;
		}
	}
}

SDL_Texture *Texturer::getTexture(std::string name) {
	//std::cout << name << "\n";
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i].name == name){
				return textureID[i].texture;
		}

	}

	std::cout << "[Texture Manager] Error : Texture \"" << name << "\" não encontrado!\n" << std::endl;
	return 0;
}

void Texturer::destroyAll() {
	for (unsigned int i = 0, end = textureID.size(); i < end; i++)
	{
		textureID[i].destroy();
		//delete textureID[i];
	}

	textureID.clear();
	delete singleton;
	singleton = nullptr;
}

SDL_Texture * Texturer::add (SDL_Renderer * renderer, std::string path)
{
	return instance()->addTexture(renderer, path);
}

SDL_Texture * Texturer::add (SDL_Renderer * renderer, std::string path, SDL_Color colorKey)
{
	return instance()->addTexture(renderer, path, colorKey);
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

std::vector<TextureID> Texturer::getTextureID()
{
	return textureID;
}

void Texturer::destroy() {
	instance()->destroyAll();
}

