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

#ifndef CHORA_LABEL_HPP
#define CHORA_LABEL_HPP

#include "widget.hpp"


#include <sstream>

class GuiLabel: public Widget
{
	private:
		SDL_Color color;
		SDL_Texture * texture;

	protected:
		std::string str;
		std::string fontName;

	protected:
		GuiLabel (  )
		{
			color = (SDL_Color)
			{
				0,0,0,255
			};
			texture = 0;
		}

	public:
		GuiLabel ( std::string s, SDL_Color c, std::string fontName="=>default" )
		{
			color = c;

			texture = 0;
			/*
				if (s != "")
					set_str(s);
				else
					str = "";
				*/
			if (s == "")
				s = " ";

			set_str(s,fontName);
			this->fontName = fontName;
		}

		~GuiLabel (  )
		{
			if (texture)
				SDL_DestroyTexture(texture);
		}

	protected:
		void str_to_surface ( std::string s, std::string fontName="=>default" );

	public:
		void set_color ( SDL_Color c );

		void set_str ( std::string s, std::string fontName="=>default" );

		SDL_Texture * get_texture (  );

		void set_texture ( SDL_Texture * t );

		int get_texture_width (  );

		int get_texture_height (  );

		std::string get_str (  );

		void draw ( SDL_Renderer * renderer );
};

/*
	NOTA: por enquanto apenas números inteiros
*/
class GuiLabelNumber: public GuiLabel
{
	protected:
		float value;
		float left_zero; // número máximo de zeros a esquerda
	protected:
		using GuiLabel::set_str;

	public:
		GuiLabelNumber ( float v, SDL_Color c, int lz=0 )
		{
			set_color(c);
			value = -1;
			left_zero = lz;
			set_value(v);
		}

	protected:
		void proc_value ( float v ); // para ser usada internamente

	public:
		void set_left_zero ( int lz );

		bool set_value ( float v );
};

/*
	GuiTextInput usada para escrever o que o usuário digitar
	O "cursor" é sempre posicionado no final da std::string
	-colocar para setar a posição do cursor com as setas direita e esquerda
	-colocar para setar a posição do cursor com o mouse
	-colocar os caracteres que precisão de shift para ser usados
*/
class GuiTextInput: public GuiLabel
{
	private:
		bool caps_lock;
		bool shift_key;
		int count;
		int textsize; // tamanho da fonte em pixels
		int strsize; // tamanho máximo da str
		SDL_Rect cursor;
	public:
		Uint32 cursor_color;

	protected:
		using Widget::set_pos;
		using Widget::set_rel_pos;

	public:
		GuiTextInput ( int fontsize, SDL_Color c, int ss=10 ): GuiLabel("", c)
		{
			if (fontsize < 1)
				throw "GuiTextInput: tamanho do texto inválido\n";

			if (ss < 1)
				throw "GuiTextInput: tamanho da std::string inválido\n";

			caps_lock = false;
			shift_key = false;
			count = 0;

			str_to_surface("a");
			textsize = get_texture_width();
			str_to_surface("");

			strsize = ss;
			cursor.x = pos.x + rel_pos.x;
			cursor.y = pos.y + rel_pos.y;
			cursor.w = 5;
			cursor.h = fontsize;
			cursor_color = 0xFF0000FF;
		}

		/*
			Define a dimensão do cursor
		*/
		void set_cursor_size ( int w, int h );

		void set_pos ( Vect p );

		void set_rel_pos ( Vect p );

		void input ( SDL_Event & event );

		void draw ( SDL_Renderer * renderer );
};

#endif

