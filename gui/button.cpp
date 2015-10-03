#include "button.hpp"

void CSimpleButton::set_callback ( void (* c) ( CWidget * b ) )
{
	callback = c;
}

void CSimpleButton::set_dim ( SDL_Rect d )
{
	dim = d;
	pos.x = d.x;
	pos.y = d.y;

	if (!label)
		return;

	SVect p;
	SDL_Rect dst;
	dst = label->get_dim();
	// posição do label relativo ao botão
	p.x = (dst.w - d.w)/2.0f;
	p.y = (dst.h - d.h)/2.0f;
	label->set_rel_pos(p);
}

void CSimpleButton::set_label ( CLabel * l )
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

	SVect p;
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

void CSimpleButton::input ( SDL_Event & event )
{
	SVect p;
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

int CSimpleButton::update (  )
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
			SVect p;
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

void CSimpleButton::draw ( SDL_Renderer * renderer )
{
	SDL_Rect d = dim;
	switch (get_state())
	{
		case 1:
			SDL_SetRenderDrawColor(renderer, (color1 & 0xFF000000) >> 24, (color1 & 0x00FF0000) >> 16, (color1 & 0x0000FF00) >> 8, (color1 & 0x000000FF));

			SDL_RenderFillRect(renderer, &d);
			break;
			
		case 2:
			SDL_SetRenderDrawColor(renderer, (color2 & 0xFF000000) >> 24, (color2 & 0x00FF0000) >> 16, (color2 & 0x0000FF00) >> 8, (color2 & 0x000000FF));

			SDL_RenderFillRect(renderer, &d);
			break;
			
		case 3:
			SDL_SetRenderDrawColor(renderer, (color3 & 0xFF000000) >> 24, (color3 & 0x00FF0000) >> 16, (color3 & 0x0000FF00) >> 8, (color3 & 0x000000FF));

			SDL_RenderFillRect(renderer, &d);
			break;
			
		default:
			SDL_SetRenderDrawColor(renderer, (color1 & 0xFF000000) >> 24, (color1 & 0x00FF0000) >> 16, (color1 & 0x0000FF00) >> 8, (color1 & 0x000000FF));

			SDL_RenderFillRect(renderer, &d);
			break;
	}

	child_draw(renderer);
}



