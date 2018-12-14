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

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>
#include <vector>

#include "../sdl.hpp"
#include "../include/statemachine.hpp"
#include "../include/vect.hpp"

class Widget: public StateMachine
{
	protected:
		std::string id; // identificação do widget
		Vect position; // posição absoluta no screen
		Vect rel_pos; // posição relativa ao widget pai
		SDL_Rect dim; // dimensão, dim.x e dim.y posição no screen
		bool visible; // só recebe eventos se estiver visível
	private:
		Widget * parent;
		std::vector <Widget *> child;

	public:
		Widget ( std::string i="" )
		{
			id = i;
			parent = 0;
			visible = true;
			setState(1);
		}

		Widget ( Widget & w )
		{
			id = w.getID();
			position = w.getPosition();
			rel_pos = w.getRelativePosition();
			dim = w.getDimension();
			visible = w.isVisible();
			parent = w.getParent();
			child.reserve(w.child.size());
			for (unsigned int i = 0; i < w.child.size(); i++)
				child[i] = w.getChild(i);
		}

		void destroy (  );

		void setID ( std::string new_id );

		std::string getID (  );

		void setPosition ( Vect p );

		Vect getPosition (  );

		void setRelativePosition ( Vect p );

		Vect getRelativePosition (  );

		void setDimension ( SDL_Rect d );

		SDL_Rect getDimension (  );

		void show ( bool s=true );

		bool isVisible (  );

		void setParent ( Widget * w );

		Widget * getParent (  );

		bool hasChild ( Widget * w );

		void showChild ( bool s=true );

		bool addChild ( Widget * w );

		void clearChildren (  );

		bool removeChild ( Widget * w );

		Widget * getChild ( std::string s_id );

		Widget * getChild ( int index );

		//NOTE: precisa testar com wigets que tenha filhos
		const std::vector<Widget *> getAllChildren();
		//pega os filhos mais perto de this
		const std::vector<Widget *> & getChildren (  );

		/*
			as funções childInput, childUpdate e childDraw devem ser chamadas nas
			respectivas funções input, update e draw isso se a classe derivada
			de Widget possuir filhos.
		*/
		void childInput ( SDL_Event & event );

		int childUpdate (  );

		void childDraw ( SDL_Renderer * renderer );



		virtual void input ( SDL_Event & event );

		virtual int update (  );

		virtual void draw ( SDL_Renderer * renderer );
};

#endif

