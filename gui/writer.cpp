#include "writer.hpp"

Writer * Writer::singleton = 0;

Writer::~Writer (  )
{
	for (std::map<std::string, Font>::iterator it = fonts.begin(); it != fonts.end(); it++)
		if (it->second.font)
		{
			TTF_CloseFont(it->second.font);
		}
	fonts.clear();
}

int Writer::load_font ( std::string path, std::string name, int s )
{
	Font font;
	if (fonts[name].font)
		TTF_CloseFont(fonts[name].font);

	font.size = s;
	font.path = path;
	font.name = name;
	font.font = TTF_OpenFont(static_cast<const char *>(path.c_str()), s);
	if (!font.font)
	{
		throw Exception("Writer::load_font não carregou uma fonta chmada "+name+" com path="+path);
	}
	
	fonts[name] = font;

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

TTF_Font * Writer::get_font ( std::string name )
{
	if (name == "default")
		return fonts.begin()->second.font;//primeira fonte é a default
	
	return fonts[name].font;
}

bool Writer::resize_font ( std::string name, int s )
{
	if (name == "default")
		name = fonts.begin()->second.name;//primeira fonte é a default
	
	Font font = fonts[name];
	if (font.font == nullptr || s <= 0)
		return false;

	TTF_CloseFont(font.font);
	font.font = TTF_OpenFont(font.path.c_str(), s);
	font.size = s;
	
	if (!font.font)
		throw Exception("Writer: não conseguiu redimensionar fonte\n");

	fonts[name] = font;

	return true;
}

void Writer::set_renderer ( SDL_Renderer * r )
{
	renderer = r;
}

SDL_Renderer * Writer::get_renderer (  )
{
	return renderer;
}


SDL_Texture * Writer::render_text ( std::string name, std::string text, SDL_Color c, int type )
{	
	if (name == "")
	{
		if (fonts.find("=>default") == fonts.end())
			throw Exception("Writer::Sem primeira fonte definida");
		name = "=>default";
	}
	
	if (text == "")
	{
		text = " "; // para evitar textura sem tamanho
		printf("Writer: sem texto visível.\n");
	}

	if (renderer == nullptr)
		throw Exception("Writer: nenhum renderer usado\n");

	SDL_Surface * surface = render_text_surface(name, text, c, type);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture)
	{
		char * e = new char[256];
		sprintf(e, "Writer: erro %s\n", SDL_GetError());
		throw Exception(e); // c++ é esquisito
	}

	return texture;
}

SDL_Surface * Writer::render_text_surface ( std::string name, std::string text, SDL_Color c, int type )
{
	if (name == "")
	{
		if (fonts.find("=>default") == fonts.end())
			throw Exception("Writer::Sem primeira fonte definida");
		name = "=>default";
	}
	
	TTF_Font * font = fonts[name].font;
	if (font == nullptr)
		throw Exception("Writer: Nenhuma "+name+" fonte usada\n");

	if (text == "")
		text = " "; // para evitar surface sem tamanho

	std::string str;
	std::vector <SDL_Surface *> tmp;
	SDL_Surface * surf = 0;
	int line = 0;

	for (unsigned int i(0); i < text.size(); i++)
	{
		if (text[i] == '\n' || i + 1 == text.size())
		{
			line++;
			if (i + 1 == text.size())
			{
				str.push_back(text[i]);
				str.push_back('\0');
			}

			SDL_Surface * surface = nullptr;
			switch (type)
			{
				case SOLID_TEXT:
					surface = TTF_RenderText_Solid(font, str.c_str(), c);
					break;
				case UTF8_TEXT:
					surface = TTF_RenderUTF8_Solid(font, str.c_str(), c);
					break;
			#ifndef EMSCRIPTEN
				case UNICODE_TEXT:
				{
					unsigned int length = str.length();
					Uint16 * s = (Uint16 *)malloc(sizeof(Uint16) * (length + 1));
					for (unsigned int i(0); i < length; i++)
						s[i] = str[i];
					s[length] = 0x0;
					surface = TTF_RenderUNICODE_Solid(font, s, c);
					free(s);
				}
				break;
			#endif
				default:
					surface = TTF_RenderText_Solid(font, str.c_str(), c);
					break;
			}

			if (surface == nullptr)
				throw Exception("Writer::Não foi possivel criar surface com texto \""+text+"\"");
			tmp.push_back(surface);
			str.clear();
		}
		else
			str.push_back(text[i]);
	}

	if (line == 1)
	{
		if (tmp.size() == 1)
			return tmp.back();
		else
			throw Exception("Writer::Erro ao criar surface");
	}

	int h = 0, w = 0;
	for (unsigned int i(0); i < tmp.size(); i++)
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

	surf = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	if(surf == nullptr)
	{
		printf("Error: %s\n",SDL_GetError());
		throw Exception(SDL_GetError());
	}

	for (unsigned int i(0); i < tmp.size(); i++)
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
		throw Exception("Writer: surface de retorno nula\n");

	return surf;
}

