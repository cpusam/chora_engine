#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../sdl.hpp"

class Renderer
{
	public:
		enum Flags: unsigned int
		{
			RENDER_DEFAULT=1,
			RENDER_ACCELERATED=2,
		};
	public:
		Renderer ( SDL_Window * window, void * renderer, unsigned int flags );

		int getId (  ) const;
		SDL_Window * getWindow (  ) const;
		unsigned int getFlags (  ) const;

		virtual void clear ( unsigned int r, unsigned int g, unsigned int b, unsigned int a );
		virtual void drawRect ( SDL_Rect & rect, unsigned int r, unsigned int g, unsigned int b, unsigned int a );
		virtual void drawTexture ( void * texture, const SDL_Rect & src, const SDL_Rect & dest );

	protected:
		//flags do renderer
		unsigned int flags;
		//ponteiro para a janela
		SDL_Window * window;
		//ponteiro para o renderer
		void * renderer;
	
	private:
		//id único do renderer
		const int id;
		//contador de ids
		static int countID;
};

#endif