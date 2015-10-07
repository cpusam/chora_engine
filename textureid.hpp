#ifndef CHORA_TEXTUREID_HPP
#define CHORA_TEXTUREID_HPP

#include "sdl.hpp"
#include <iostream>
#include <string>

struct TextureID{
	SDL_Texture *texture;
	std::string path;
	std::string name;

	TextureID();
	TextureID(SDL_Texture *tex, std::string n);
	TextureID(std::string path, SDL_Renderer *renderer);
	virtual ~TextureID();
	virtual void destroy();
};

#endif
 
