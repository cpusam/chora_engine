#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <vector>
#include <map>

#include "../Chora.hpp"
#include "Entity.hpp"

class Entity;
typedef unsigned long int EntityID;

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
		static std::map<EntityID, Entity *> getAllEntities (  );
		//retorna todas entidades do group e sem nenhuma nulaou de id nulo
		static std::vector<Entity *> getAllByGroup ( std::string group ); // pega objeto pelo grupo
		static Entity * remove ( EntityID id ); //remove uma entity
		static void clear (  );
		static void destroy (  );

		static void notifyGroup ( Entity *sender, std::string mesg, std::string group );
		
		static void input ( SDL_Event & event );
		static void draw ( SDL_Renderer * renderer, Camera * camera );
		static void update (  );
		
		static void print (  );


	
	private:
		SDL_Renderer * currRenderer;
		std::map<EntityID, Entity *> entities;
		static Elements * singleton;
		Elements();

		void setCurrRenderer ( SDL_Renderer * renderer );
		SDL_Renderer * getCurrRenderer (  );
		bool hasEntity ( Entity * e );
		std::map<EntityID, Entity *> getEntities (  );
		void addEntity ( Entity * e );
		Entity * getEntity ( EntityID id );
		Entity * getEntityByName ( std::string name );
		std::vector<Entity *> getAllEntityByGroup ( std::string group );
		Entity * remEntity ( EntityID id );
		void clearAll (  );

		void inputEntities ( SDL_Event & event );
		void drawEntities ( SDL_Renderer * renderer, Camera * camera );
		void updateEntities (  );
};

#endif // ELEMENTS_HPP
