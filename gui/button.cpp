#include "button.hpp"

GuiButton::GuiButton ( SDL_Rect d )
{
	label = nullptr;
	callback = nullptr;
	this->texture = nullptr;
	color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
	color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
	color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
	position.x = d.x, position.y = d.y;
	// dimensão padrão
	dim = d;
	setState(State::NORMAL);
	run_release = false;
	texture = nullptr;
}

GuiButton::GuiButton ( SDL_Rect d, std::string str, std::string fontName, SDL_Rect * src, SDL_Texture * texture )
{
	label = nullptr;
	callback = nullptr;
	color1 = (SDL_Color){0xFF, 0xFF, 0x00, 0xFF};
	color2 = (SDL_Color){0x00, 0xFF, 0xFF, 0xFF};
	color3 = (SDL_Color){0xFF, 0x00, 0x00, 0xFF};
	position.x = d.x, position.y = d.y;
	// dimensão padrão
	dim = d;
	set_label(new GuiLabel(str, (SDL_Color){0,0,0,255}, fontName)); // por padrão na cor preta
	setState(State::NORMAL);
	this->texture = nullptr;
	setTexture(texture);
	setSourceRects(src);
	run_release = false;
}

GuiButton::~GuiButton (  )
{
	//widget::destroy trata de dar conta de label, que foi adicionada com addChild
}

void GuiButton::press (  )
{
	if (getState() != State::PRESSED)
		setState(State::PRESSED);
}

void GuiButton::select (  )
{
	if (getState() != State::SELECTED)
		setState(State::SELECTED);
}

void GuiButton::release (  )
{
	if (getState() != State::RELEASED)
	{
		setState(State::RELEASED);
		run_release = false;
	}
}

void GuiButton::setTexture ( SDL_Texture * texture )
{
	if (this->texture)
	{
		SDL_DestroyTexture(this->texture);
		this->texture = nullptr;
	}
	this->texture = texture;
}

void GuiButton::setSourceRects ( SDL_Rect src[3] )
{
	if (src)
	{
		this->src[0] = src[0];
		this->src[1] = src[1];
		this->src[2] = src[2];
	}
	else
	{
		this->src[0] = (SDL_Rect){0,0,1,1};
		this->src[1] = this->src[0];
		this->src[2] = this->src[0];
	}
}

SDL_Texture * GuiButton::getTexture (  )
{
	return texture;
}

GuiLabel * GuiButton::get_label (  )
{
	return label;
}

void GuiButton::set_callback ( void (* c) ( Widget * b ) )
{
	callback = c;
}

void GuiButton::setDimension ( SDL_Rect d )
{
	dim = d;
	position.x = d.x;
	position.y = d.y;

	if (!label)
		return;

	Vect p;
	SDL_Rect dst;
	dst = label->getDimension();
	// posição do label relativo ao botão
	p.x = (dst.w - d.w)/2.0f;
	p.y = (dst.h - d.h)/2.0f;
	label->setRelativePosition(p);
}

void GuiButton::set_label ( GuiLabel * l )
{
	if (!l)
		return;

	if (label && label != l)
	{
		removeChild(label);
		delete label;
		label = nullptr;
		throw 1;
	}


	label = l;
	addChild(label);

	Vect p;
	SDL_Rect d;
	int w, h;
	d = label->getDimension();
	w = d.w, d.w += 10;
	h = d.h, d.h += 5;
	dim.w = d.w, dim.h = d.h; // tamanho do botão
	// posição do label relativo ao botão
	p.x = (d.w - w)/2.0f;
	p.y = (d.h - h)/2.0f;
	label->setRelativePosition(p);
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
				p.set(event.button.x, event.button.y);
				if (getState() == State::SELECTED || getState() == State::NORMAL)
				{
					if (event.button.state == SDL_PRESSED && pointbox(p, getDimension()))
						setState(State::PRESSED);
				}
				else if (getState() == State::RELEASED)
				{
					if (!run_release && pointbox(p, getDimension()))
					{
						setState(State::PRESSED);
					}	
				}
			}
			break;
			
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				p.set(event.button.x, event.button.y);
				if (getState() == State::PRESSED)
					if (event.button.state == SDL_RELEASED)
					{
						run_release = true;
						setState(State::RELEASED);
					}
			}
			break;
		
		case SDL_MOUSEMOTION:
			p.x = event.motion.x;
			p.y = event.motion.y;
			if (pointbox(p, dim))
			{
				if (getState() == State::NORMAL || getState() == State::RELEASED)
					setState(State::SELECTED);
			}
			else
			{
				setState(State::NORMAL);
			}
			break;
		default:
			break;
	}

	childInput(event);
}

int GuiButton::update (  )
{
	int x, y;
	Vect p;
	SDL_GetMouseState(&x, &y);
	p.x = x, p.y = y;

	switch (getState())
	{
		case State::NORMAL: // normal
			break;
		case State::SELECTED: // selecionado
			break;
		case State::PRESSED: // pressionado
			break;
		case State::RELEASED: // solto depois de pressionado
		{
			if (getState() == run_release)
			{
				if (callback)
					callback(this);
				run_release = false;
			}
			if (pointbox(p, getDimension()))
				setState(State::SELECTED);
			else
				setState(State::NORMAL);
			break;
		}
		default:
			break;
	}

	childUpdate();
	
	return getState();
}

void GuiButton::draw ( SDL_Renderer * renderer )
{
	if (!visible)
		return;
	
	if (!texture)
	{
		SDL_Rect rect = dim;
		switch (getState())
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
		childDraw(renderer);
		return;
	}
	
	SDL_Rect d = dim;
	switch (getState())
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

	childDraw(renderer);
}


