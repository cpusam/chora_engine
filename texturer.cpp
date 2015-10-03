#include "texturer.hpp"

CTexturer * CTexturer::singleton = NULL;

CTexturer::CTexturer(){}


CTexturer::~CTexturer(){}

CTexturer* CTexturer::instance(){
	if( singleton == NULL ){
		singleton = new CTexturer();
	}
	return singleton;
}

void CTexturer::add (SDL_Renderer * renderer, std::string path){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->path == path){
			rem(textureID[i]->name);
			textureID.push_back(new STextureID(path,renderer));
			return;
		}
	}

	textureID.push_back(new STextureID(path,renderer));
}

void CTexturer::add (SDL_Texture *tex, std::string name){
	for(unsigned int i = 0;i<textureID.size();i++){
		if(textureID[i]->texture == tex){
			
			rem(textureID[i]->name);
			textureID.push_back(new STextureID(tex, name));
			return;
		}
	}
	
	textureID.push_back(new STextureID(tex, name));
}

void CTexturer::rem (std::string name){
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
			textureID[i]->destroy();
			textureID.erase(textureID.begin() + i);
			break;
		}
	}
}

SDL_Texture *CTexturer::get_texture(std::string name) {
	//std::cout << name << "\n";
	for (unsigned int i = 0, end = textureID.size(); i < end; i++){
		if(textureID[i]->name == name){
				return textureID[i]->texture;
		}

	}

	std::cout << "[Texture Manager] Error : Texture " << name << " not found !" << std::endl;
	throw "[TextureManager] Error : Texture not found!";
}

void CTexturer::destroy() {
	for (unsigned int i = 0, end = textureID.size(); i < end; i++)
		delete textureID[i];

	textureID.clear();
}

