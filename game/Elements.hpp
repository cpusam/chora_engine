/*
 Copyright (C) 2014-2018 Samuel Leonardo

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

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <vector>
#include <map>
#include <mutex>
#include <atomic>

#include "Entity.hpp"

class Entity;

class Elements
{
	public:
		virtual ~Elements();
		
		static Elements * instance (  );
		static void setRenderer ( SDL_Renderer * renderer );
		static SDL_Renderer * getRenderer (  );
		
		static bool has ( Entity * e );
		static void add ( Entity * e ); // adiciona uma entidade
		static Entity * get ( EntityID id ); // pega uma entidade pelo id dela
		static Entity * getByName ( std::string name ); // pega objeto pelo nome
		static std::vector<Entity *> getAllEntities (  );
		//retorna todas entidades do group e sem nenhuma nulaou de id nulo
		static std::vector<Entity *> getAllByGroup ( std::string group ); // pega objeto pelo grupo
		static Entity * remove ( EntityID id ); //remove uma entity
		static void clear (  );
		static void destroy (  );

		//se group == "ALL" notifica todo mundo
		static void notifyGroup ( Entity *sender, std::string mesg, std::string group );
		
		static void input ( SDL_Event & event );
		static void draw ( SDL_Renderer * renderer, Camera * camera );
		static void update (  );
		
		static void print (  );


	
	private:
		SDL_Renderer * currRenderer;
		std::vector<EntityID> entitiesID;
		std::vector<Entity *> entities;
		static std::atomic<Elements *> singleton;
		static std::mutex myMutex;
		Elements();

		void setCurrRenderer ( SDL_Renderer * renderer );
		SDL_Renderer * getCurrRenderer (  );
		bool hasEntity ( Entity * e );
		std::vector<Entity *> getEntities (  );
		void addEntity ( Entity * e );
		Entity * getEntity ( EntityID id );
		Entity * getEntityByName ( std::string name );
		std::vector<Entity *> getAllEntityByGroup ( std::string group );
		Entity * remEntity ( EntityID id );
		void clearAll (  );
		std::vector<EntityID> getIDs (  );

		void inputEntities ( SDL_Event & event );
		void drawEntities ( SDL_Renderer * renderer, Camera * camera );
		void updateEntities (  );
};

#endif // ELEMENTS_HPP
