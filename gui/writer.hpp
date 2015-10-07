/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#ifndef CHORA_WRITER_HPP
#define CHORA_WRITER_HPP

#include <string>
#include <vector>

#include "../sdl.hpp"
#include "../util.hpp"

struct SText
{
	std::string text;
	SDL_Color color;
};

enum ETextType
{
	SOLID_TEXT,
	UTF8_TEXT,
	UNICODE_TEXT
};

/*
	classe para escrever linha de texto com a SDL_ttf
*/
class Writer
{
	private:
		int size; // tamanho da fonte
		std::string path; // caminho para a fonte
		TTF_Font * font;
		SDL_Renderer * renderer;
		static Writer * singleton;

		Writer (  )
		{
			font = 0;
			size = 0;
			renderer = 0;

	#ifndef __clang__
			if (!TTF_WasInit())
				throw "Writer: SDL_ttf não inicializada\n";
	#endif
		}

		~Writer (  )
		{
			if (singleton)
			{
				if (font)
					TTF_CloseFont(font);
			}
		}

	public:
		static Writer * instance (  )
		{
			if (!singleton)
				singleton = new Writer();

			return singleton;
		}



		int set_font ( std::string p, int s );

		static void destroy (  );

		TTF_Font * get_font (  );

		bool resize_font ( int s );

		void set_renderer ( SDL_Renderer * r );

		SDL_Renderer * get_renderer (  );


		SDL_Texture * render_text ( std::string text, SDL_Color c, int type=SOLID_TEXT );

		SDL_Surface * render_text_surface ( std::string text, SDL_Color c, int type=SOLID_TEXT );

		/*
			Para colorir um texto basta usar as tags <color=R,G,B> TEXTO AQUI </color>
		*/
		SDL_Surface * render_colored_text ( std::string text, SDL_Color color, int type=SOLID_TEXT );
};

#endif

