#ifndef KEY_HPP
#define KEY_HPP

#include "../sdl.hpp"
#include "../statemachine.hpp"

class Key: public StateMachine
{
	public:
		enum KeyState: int
		{
			FREE,
			PRESS,
			HOLD,
			RELEASE,
		};

	public:
		Key();
		Key ( SDL_Keycode k );
		virtual ~Key();
		
		Uint32 getPressTime (  );

		bool isPressed();
		void setKey ( SDL_Keycode k );
		SDL_Keycode getKey (  );
		
		virtual void reset (  );
		virtual void input ( SDL_Event & event );
		virtual int update (  );
	
	protected:
		SDL_Keycode key;
		bool press;
		
	private:
		Uint32 pressTime;
};

#endif // KEY_HPP
