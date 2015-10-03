#ifndef CHORA_TEXTUREID_HPP
#define CHORA_TEXTUREID_HPP

#include "sdl.hpp"
#include <iostream>
#include <string>

struct STextureID{
	SDL_Texture *texture;
	std::string path;
	std::string name;

	STextureID();
	STextureID(SDL_Texture *tex, std::string n);
	STextureID(std::string path, SDL_Renderer *renderer);
	virtual ~STextureID();
	virtual void destroy();
};

#endif
 
