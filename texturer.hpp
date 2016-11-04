#ifndef CHORA_TEXTURER_HPP
#define CHORA_TEXTURER_HPP

#include <vector>

#include "textureid.hpp"


class Texturer{
	std::vector<TextureID>	 textureID;
	static Texturer 		*singleton;

	Texturer();
	virtual ~Texturer();
	
	void destroyAll (  );
	void addTexture ( SDL_Texture *tex, std::string name );
	SDL_Texture * addTexture (SDL_Renderer *renderer, std::string path);
	SDL_Texture * addTexture (SDL_Renderer * renderer, std::string path, SDL_Color colorKey);
	void remTexture ( std::string name );
	SDL_Texture* getTexture ( std::string name );
	
	public:
		static Texturer *instance();
		
		static SDL_Texture * add (SDL_Renderer * renderer, std::string path);
		static SDL_Texture * add (SDL_Renderer * renderer, std::string path, SDL_Color colorKey );
		static void add (SDL_Texture *tex, std::string name );
		static void rem ( std::string name );
		static SDL_Texture* get ( std::string name );
		std::vector<TextureID> getTextureID();
		static void destroy();
};

#endif // CHORA_TEXTUREMANAGER_H
