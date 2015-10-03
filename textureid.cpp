#include "textureid.hpp"

STextureID::STextureID(){
	path = "";
	name = "";
	texture = NULL;
}

STextureID::STextureID(SDL_Texture *tex, std::string n){
	path = "";
	name = n;
	texture = tex;
	
	if (tex == NULL)
	{
		throw "Erro: ao setar textura NULA";
	}
}

STextureID::STextureID(std::string path, SDL_Renderer *renderer){
	this->path = path;

	unsigned int begin = this->path.find_last_of("/");
	if(begin == std::string::npos)
		this->name = this->path; // aqui é sem '/'
	else
		this->name = this->path.substr(begin); // aqui é com barra

	texture = IMG_LoadTexture(renderer,path.c_str());
	
	if(texture == NULL){
		std::string ret = "[Texture ID] Erro Criando Textura : " + std::string(SDL_GetError());
		
		throw ret.c_str();
	}
}

STextureID::~STextureID(){
	destroy();
}

void STextureID::destroy(){
	if(texture != NULL)
		SDL_DestroyTexture(texture);
	texture = NULL;
	path = "";
	name = "";
}
