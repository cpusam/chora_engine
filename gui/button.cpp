#include "button.hpp"

void GuiButton::set_callback ( void (* c) ( Widget * b ) )
{
	callback = c;
}

void GuiButton::set_dim ( SDL_Rect d )
{
	dim = d;
	pos.x = d.x;
	pos.y = d.y;

	if (!label)
		return;

	Vect p;
	SDL_Rect dst;
	dst = label->get_dim();
	// posição do label relativo ao botão
	p.x = (dst.w - d.w)/2.0f;
	p.y = (dst.h - d.h)/2.0f;
	label->set_rel_pos(p);
}

void GuiButton::set_label ( GuiLabel * l )
{
	if (!l)
		return;

	if (label)
	{
		rem_child(label);
		delete label;
	}


	label = l;
	add_child(label);

	Vect p;
	SDL_Rect d;
	int w, h;
	d = label->get_dim();
	w = d.w, d.w += 10;
	h = d.h, d.h += 5;
	dim.w = d.w, dim.h = d.h; // tamanho do botão
	// posição do label relativo ao botão
	p.x = (d.w - w)/2.0f;
	p.y = (d.h - h)/2.0f;
	label->set_rel_pos(p);
}

void GuiButton::input ( SDL_Event & event )
{
	Vect p;
	switch (event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (get_state() == 2)
					if (event.button.state == SDL_PRESSED)
						set_state(3);
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (get_state() == 3)
					if (event.button.state == SDL_RELEASED)
						set_state(4);
			}
			break;

		case SDL_MOUSEMOTION:
			p.x = event.motion.x;
			p.y = event.motion.y;
			if (pointbox(p, dim))
			{
				if (get_state() == 1)
					set_state(2);
			}
			else
			{
				set_state(1);
			}
			break;
		default:
			break;
	}
}

int GuiButton::update (  )
{
	switch (get_state())
	{
		case 1: // normal
			return 1;
		case 2: // selecionado
			return 2;
		case 3: // pressionado
			return 3;
		case 4:   // solto depois de pressionado
		{
			if (callback)
				callback(this);

			int x, y;
			Vect p;
			SDL_GetMouseState(&x, &y);
			p.x = x, p.y = y;

			if (pointbox(p, dim))
				set_state(2);

			return 4;
		}
		default:
			break;
	}

	return get_state();
}

void GuiButton::draw ( SDL_Renderer * renderer )
{
	SDL_Rect d = dim;
	switch (get_state())
	{
		case 1:
			SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a);

			SDL_RenderFillRect(renderer, &d);
			break;
			
		case 2:
			SDL_SetRenderDrawColor(renderer, color2.r, color2.g, color2.b, color2.a);

			SDL_RenderFillRect(renderer, &d);
			break;
			
		case 3:
			SDL_SetRenderDrawColor(renderer, color3.r, color3.g, color3.b, color3.a);

			SDL_RenderFillRect(renderer, &d);
			break;
			
		default:
			SDL_SetRenderDrawColor(renderer, color1.r, color1.g, color1.b, color1.a);

			SDL_RenderFillRect(renderer, &d);
			break;
	}

	child_draw(renderer);
}



