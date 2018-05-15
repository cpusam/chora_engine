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

#ifndef CHORA_BUTTON_HPP
#define CHORA_BUTTON_HPP

#include "widget.hpp"
#include "label.hpp"
#include "../include/collision.hpp"

/*
	Botão simples de 4 estados, normal, selecionado, pressionado e solto depois de press
*/
class GuiButton: public Widget
{
	protected:
		void (* callback) ( Widget * b );
		GuiLabel * label;

	public:
		SDL_Color color1; // cor normal
		SDL_Color color2; // cor de selecionado
		SDL_Color color3; // cor de pressionado

	public:
		GuiButton ( SDL_Rect d )
		{
			label = 0;
			callback = 0;
			color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
			color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
			color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
			pos.x = d.x, pos.y = d.y;
			// dimensão padrão
			dim = d;
			set_state(1);
		}

		GuiButton ( SDL_Rect d, std::string str )
		{
			label = 0;
			callback = 0;
			color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
			color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
			color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
			pos.x = d.x, pos.y = d.y;
			// dimensão padrão
			dim = d;
			set_label(new GuiLabel(str, (SDL_Color){0,0,0,0})); // por padrão na cor preta
			set_state(1);
		}

		~GuiButton (  )
		{
			if (label)
				delete label;
		}

		virtual void set_callback ( void (* c) ( Widget * b ) );

		virtual void set_dim ( SDL_Rect d );

		virtual void set_label ( GuiLabel * l );

		virtual void input ( SDL_Event & event );

		virtual int update (  );

		virtual void draw ( SDL_Renderer * renderer );
};

#endif


