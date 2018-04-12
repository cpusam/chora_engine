#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

#include <vector>
#include <map>

#include "../Chora.hpp"
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
		static Entity * get ( int id ); // pega uma entidade pelo id dela
		static Entity * getByName ( std::string name ); // pega objeto pelo nome
		static std::map<int, Entity *> & getAllEntities (  );
		static std::vector<Entity *> getAllByGroup ( std::string group ); // pega objeto pelo grupo
		static void remove ( int id ); //remove uma entity
		static void clear (  );
		static void destroy (  );

		static void notifyGroup ( Entity *sender, std::string mesg, std::string group );
		
		static void input ( SDL_Event & event );
		static void draw ( SDL_Renderer * renderer, Camera * camera );
		static void update (  );
		
		static void print (  );


	
	private:
		SDL_Renderer * currRenderer;
		std::map<int, Entity *> entities;
		static Elements * singleton;
		Elements();

		void setCurrRenderer ( SDL_Renderer * renderer );
		SDL_Renderer * getCurrRenderer (  );
		bool hasEntity ( Entity * e );
		std::map<int, Entity *> & getEntities (  );
		void addEntity ( Entity * e );
		Entity * getEntity ( int id );
		Entity * getEntityByName ( std::string name );
		std::vector<Entity *> getAllEntityByGroup ( std::string group );
		void remEntity ( int id );
		void clearAll (  );
};

#endif // ELEMENTS_HPP
