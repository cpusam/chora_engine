#ifndef CAMERARECT_HPP
#define CAMERARECT_HPP

#include "../../include/camera.hpp"
#include "../../sdl.hpp"

class CameraRectFocus: public Camera
{
	public:
		CameraRectFocus ( SDL_Renderer * renderer, SDL_Rect viewport, SDL_Rect limit, SDL_Rect rectFocus );
		virtual ~CameraRectFocus(){}
		void setRectFocus ( SDL_Rect rect );
		SDL_Rect getRectFocus (  );
		virtual void lookat ( Vect p );
		//retorna verdadeiro caso chegue ao 'p'
		bool moveToPosition ( Vect & p, float maxVel );

	protected:
		using Camera::setFocus;
		using Camera::getFocus;
		Vect velocity;
		SDL_Rect rectFocus;
};

#endif
