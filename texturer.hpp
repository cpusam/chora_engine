#ifndef CHORA_TEXTURER_HPP
#define CHORA_TEXTURER_HPP

#include <vector>

#include "textureid.hpp"


class Texturer{

	std::vector<TextureID *>	 textureID;
	static Texturer 		*singleton;

	Texturer();
	virtual ~Texturer();

	public:
		static Texturer *instance();

		void add (SDL_Renderer *renderer, std::string path);
		void add (SDL_Texture *tex, std::string name );
		void rem (std::string name);
		SDL_Texture* get_texture(std::string name);
		void destroy();
};

#endif // CHORA_TEXTUREMANAGER_H
