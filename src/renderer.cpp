#include "renderer.hpp"

int Renderer::countID = 0;

Renderer::Renderer ( SDL_Window * window, void * renderer, unsigned int flags )
:id(countID++)
{
	this->window = window;
	this->flags = flags;
	this->renderer = renderer;
}

int Renderer::getId (  ) const
{
	return id;
}

SDL_Window * Renderer::getWindow (  ) const
{
	return window;
}

unsigned int Renderer::getFlags (  ) const
{
	return flags;
}

void Renderer::clear ( unsigned int r, unsigned int g, unsigned int b, unsigned int a )
{

}

void Renderer::drawRect ( SDL_Rect & rect, unsigned int r, unsigned int g, unsigned int b, unsigned int a )
{
	//
}

void Renderer::drawTexture ( void * texture, const SDL_Rect & src, const SDL_Rect & dest )
{
	//
}