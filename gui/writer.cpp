#include "writer.hpp"

Writer * Writer::singleton = 0;

int Writer::set_font ( std::string p, int s )
{
	if (s > 0 && s == size && p == path)
		return -1;

	if (font)
		TTF_CloseFont(font);

	size = s;
	path = p;
	font = TTF_OpenFont(static_cast<const char *>(p.c_str()), s);
	if (!font)
	{
		return 0;
	}

	return 1;
}

void Writer::destroy (  )
{
	if (singleton)
	{
		delete singleton;
		singleton = 0;
	}
}

TTF_Font * Writer::get_font (  )
{
	return font;
}

bool Writer::resize_font ( int s )
{
	if (!font || s <= 0)
		return false;

	TTF_CloseFont(font);
	font = TTF_OpenFont(path.c_str(), s);

	if (!font)
		throw "Writer: não conseguiu redimensionar fonte\n";

	return true;
}

#if USE_SDL2
void Writer::set_renderer ( SDL_Renderer * r )
{
	renderer = r;
}

SDL_Renderer * Writer::get_renderer (  )
{
	return renderer;
}


SDL_Texture * Writer::render_text ( std::string text, SDL_Color c, int type )
{
	if (text == "")
	{
		text = " "; // para evitar textura sem tamanho
		printf("Writer: sem texto visível.\n");
	}

	if (renderer == 0)
		throw "Writer: nenhum renderer usado\n";

	SDL_Surface * surface = render_text_surface(text, c, type);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture)
	{
		char * e = new char[256];
		sprintf(e, "Writer: erro %s\n", SDL_GetError());
		throw (char *)e; // c++ é esquisito
	}

	return texture;
}
#endif

SDL_Surface * Writer::render_text_surface ( std::string text, SDL_Color c, int type )
{
	if (font == 0)
		throw "Writer: Nenhuma fonte usada\n";

	if (text == "")
		text = " "; // para evitar surface sem tamanho

	std::string str;
	std::vector <SDL_Surface *> tmp;
	SDL_Surface * surf = 0;
	int line = 0;

	for (int i(0); i < text.size(); i++)
	{
		if (text[i] == '\n' || i + 1 == text.size())
		{
			line++;
			if (i + 1 == text.size())
			{
				str.push_back(text[i]);
				str.push_back('\0');
			}

			switch (type)
			{
				case SOLID_TEXT:
					tmp.push_back(TTF_RenderText_Solid(font, str.c_str(), c));
					break;
				case UTF8_TEXT:
					tmp.push_back(TTF_RenderUTF8_Solid(font, str.c_str(), c));
					break;
			#ifndef __clang__
				case UNICODE_TEXT:
				{
					Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (str.length() + 1));
					for (int i(0); i < str.length(); i++)
						s[i] = str[i];
					s[str.length()] = 0x0;
					tmp.push_back(TTF_RenderUNICODE_Solid(font, s, c));
					free(s);
				}
				break;
			#endif
				default:
					tmp.push_back(TTF_RenderText_Solid(font, str.c_str(), c));
					break;
			}

			str.clear();
		}
		else
			str.push_back(text[i]);
	}

	if (line == 1)
	{
		switch (type)
		{
			case SOLID_TEXT:
				return TTF_RenderText_Solid(font, text.c_str(), c);

			case UTF8_TEXT:
				return TTF_RenderUTF8_Solid(font, text.c_str(), c);
		#ifndef __clang__
			case UNICODE_TEXT:
			{
				Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (str.length() + 1));
				for (int i(0); i < str.length(); i++)
					s[i] = str[i];
				s[str.length()] = '\0';
				SDL_Surface * srf = TTF_RenderUNICODE_Solid(font, s, c);
				free(s);
				return srf;
			}
			break;
		#endif
			default:
				return TTF_RenderText_Solid(font, text.c_str(), c);
		}
	}

	int h = 0, w = 0;
	for (int i(0); i < tmp.size(); i++)
	{
		h += tmp[i]->h;
		if (tmp[i]->w > w)
			w = tmp[i]->w;
	}

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
								rmask, gmask, bmask, amask);
	if(surf == 0)
		throw SDL_GetError();

	for (int i(0); i < tmp.size(); i++)
	{
		SDL_Rect d;
		d.x = 0;
		d.y = tmp[i]->h * i;
		d.w = tmp[i]->w;
		d.h = tmp[i]->h;
		SDL_BlitSurface(tmp[i], 0, surf, &d);
		SDL_FreeSurface(tmp[i]);
	}

	if (!surf)
		throw "Writer: surface de retorno nula\n";

	return surf;
}

