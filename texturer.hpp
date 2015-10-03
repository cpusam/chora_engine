#ifndef CHORA_TEXTURER_HPP
#define CHORA_TEXTURER_HPP

#include <vector>

#include "textureid.hpp"


class CTexturer{

	std::vector<STextureID *>	 textureID;
	static CTexturer 		*singleton;

	CTexturer();
	virtual ~CTexturer();

	public:
		static CTexturer *instance();

		void add (SDL_Renderer *renderer, std::string path);
		void add (SDL_Texture *tex, std::string name );
		void rem (std::string name);
		SDL_Texture* get_texture(std::string name);
		void destroy();
};

#endif // CHORA_TEXTUREMANAGER_H
