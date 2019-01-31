#include "texturer.hpp"
#include "Exception.hpp"
#include "../game/Elements.hpp"

Texturer * Texturer::singleton = nullptr;
SDL_Texture * Texturer::defaultTexture = nullptr;

Texturer::Texturer()
{
	if (defaultTexture == nullptr)
	{
		if (Elements::getRenderer() == nullptr)
			return;
		
		defaultTexture = SDL_CreateTexture(Elements::getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 32, 32);
		if (!defaultTexture)
			std::cerr<<"Texturer::erro ao criar defaultTexture: "<<SDL_GetError()<<std::endl;
	}
}

Texturer::~Texturer(){}

Texturer* Texturer::instance(){
	if(!singleton)
			singleton = new Texturer();
	
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
	{
		printf("Texturer::%s\n",IMG_GetError());
		return;
	}
	
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

SDL_Texture *Texturer::getTexture(std::string name, bool throwOnError) {
	//std::cout << name << "\n";
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i].name == name){
				return textureID[i].texture;
		}

	}

	if (throwOnError)
		throw Exception("[Texture Manager] Error : Texture \"" + name + "\" não encontrada");
	std::cerr << "[Texture Manager] Error : Texture \"" + name + "\" não encontrada";
	return defaultTexture;
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

SDL_Texture* Texturer::get ( std::string name, bool throwOnError )
{
	return instance()->getTexture(name, throwOnError);
}

std::vector<TextureID> Texturer::getTextureID()
{
	return textureID;
}

void Texturer::destroy() {
	instance()->destroyAll();
}

