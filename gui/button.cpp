#include "button.hpp"

GuiButton::GuiButton ( SDL_Rect d )
{
	label = 0;
	callback = 0;
	color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
	color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
	color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
	pos.x = d.x, pos.y = d.y;
	// dimensão padrão
	dim = d;
	set_state(State::NORMAL);
	run_release = false;
	texture = nullptr;
}

GuiButton::GuiButton ( SDL_Rect d, std::string str )
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
	set_state(State::NORMAL);
	run_release = false;
	texture = nullptr;
}

GuiButton::GuiButton ( SDL_Rect d, std::string str, SDL_Rect * src, SDL_Texture * texture )
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
	set_state(State::NORMAL);
	set_texture(texture);
	set_sources(src);
	run_release = false;
}

GuiButton::~GuiButton (  )
{
	if (label)
		delete label;
}

void GuiButton::doPress (  )
{
	if (get_state() != State::PRESSED)
		set_state(State::PRESSED);
}

void GuiButton::doSelect (  )
{
	if (get_state() != State::SELECTED)
		set_state(State::SELECTED);
}

void GuiButton::doRelease (  )
{
	if (get_state() != State::RELEASED)
		set_state(State::RELEASED);
}

void GuiButton::set_texture ( SDL_Texture * texture )
{
	this->texture = texture;
}

void GuiButton::set_sources ( SDL_Rect src[3] )
{
	this->src[0] = src[0];
	this->src[1] = src[1];
	this->src[2] = src[2];
}

SDL_Texture * GuiButton::get_texture (  )
{
	return texture;
}


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
	if (!visible)
		return;

	Vect p;
	switch (event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (get_state() == State::SELECTED)
					if (event.button.state == SDL_PRESSED)
						set_state(PRESSED);
			}
			break;
			
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (get_state() == State::PRESSED)
					if (event.button.state == SDL_RELEASED)
					{
						run_release = true;
						set_state(State::RELEASED);
					}
			}
			break;
		
		case SDL_MOUSEMOTION:
			p.x = event.motion.x;
			p.y = event.motion.y;
			if (pointbox(p, dim))
			{
				if (get_state() == State::NORMAL)
					set_state(State::SELECTED);
			}
			else
			{
				set_state(State::NORMAL);
			}
			break;
		default:
			break;
	}
}

int GuiButton::update (  )
{
	if (!visible)
		return -1;

	switch (get_state())
	{
		case State::NORMAL: // normal
			break;
		case State::SELECTED: // selecionado
			break;
		case State::PRESSED: // pressionado
			break;
		case State::RELEASED: // solto depois de pressionado
		{
			int x, y;
			Vect p;
			SDL_GetMouseState(&x, &y);
			p.x = x, p.y = y;

			if (pointbox(p, dim) && !run_release)
				set_state(State::SELECTED);

			run_release = false;
			break;
		}
		default:
			break;
	}
	
	return get_state();
}

void GuiButton::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;
	
	if (!texture)
	{
		SDL_Rect rect = dim;
		switch (get_state())
		{
			case State::NORMAL:
				SDL_SetRenderDrawColor(renderer, color1.r, color1.b, color1.g, color1.a);
				break;
			case State::SELECTED:
				SDL_SetRenderDrawColor(renderer, color2.r, color2.b, color2.g, color2.a);
				break;
			case State::PRESSED:
				SDL_SetRenderDrawColor(renderer, color3.r, color3.b, color3.g, color3.a);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, color1.r, color1.b, color1.g, color1.a);
				break;
		}
		SDL_RenderFillRect(renderer, &rect);
		return;
	}
	
	SDL_Rect d = dim;
	switch (get_state())
	{
		case State::NORMAL:
			if (SDL_RenderCopy(renderer,texture, &src[0], &d) == -1)
				throw SDL_GetError();
			break;
		case State::SELECTED:
			if (SDL_RenderCopy(renderer,texture, &src[1], &d) == -1)
				throw SDL_GetError();
			break;
		case State::PRESSED:
			if (SDL_RenderCopy(renderer,texture, &src[2], &d) == -1)
				throw SDL_GetError();
			break;
		default:
			if (SDL_RenderCopy(renderer,texture, &src[0], &d) == -1)
				throw SDL_GetError();
			break;
	}

	child_draw(renderer);
}


