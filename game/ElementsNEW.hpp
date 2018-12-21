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

#ifndef ELEMENTS_NEW_HPP
#define ELEMENTS_NEW_HPP

#include <vector>
#include <map>

#include "Chora.hpp"
#include "Node.hpp"

#define ALL (unsigned int)(-1)

class Elements
{
	public:
		virtual ~Elements();
		
		static Elements * instance (  );
		static void setRenderer ( SDL_Renderer * renderer );
		static SDL_Renderer * getRenderer (  );
		static Camera * getCamera (  );
		static void setCamera ( Camera * cam );
		
		static bool has ( Node * e );
		static void add ( Node * e ); // adiciona uma entidade
		static Node * get ( NodeID id ); // pega uma entidade pelo id dela
		static Node * getByName ( const std::string & name ); // pega objeto pelo nome
		static std::vector<Node *> getAllNodes (  );
		//retorna todas entidades do group e sem nenhuma nulaou de id nulo
		static std::vector<Node *> getAllByGroup ( const NodeGroups groups ); // pega objeto pelo grupo
		static Node * remove ( NodeID id ); //remove uma Node
		static void clear (  );
		static void destroy (  );

		//ALL notifica todo mundo
		static void notifyGroup ( Node *sender, const std::string & mesg, const NodeGroups groups=ALL );
		
		static void input ( SDL_Event & event );
		static void draw ( SDL_Renderer * renderer, Camera * camera );
		static void update (  );
		
		static void print (  );


	
	private:
		Elements();
		void inputNodes ( SDL_Event & event );
		void drawNodes ( SDL_Renderer * renderer, Camera * camera );
		void updateNodes (  );

		void setCurrRenderer ( SDL_Renderer * renderer );
		void setCurrCamera ( Camera * cam );
		SDL_Renderer * getCurrRenderer (  );
		Camera * getCurrCamera (  );

		bool hasNode ( Node * e );
		std::vector<Node *> getNodes (  );
		void addNode ( Node * e );
		Node * getNode ( NodeID id );
		Node * getNodeByName ( const std::string & name );
		std::vector<Node *> getAllNodeByGroup ( const NodeGroups group );
		Node * remNode ( NodeID id );
		void clearAll (  );
		const std::map<int, std::vector<Node *> > & getNodeLayers (  );

		std::map<int, std::vector<Node *> > layers;
		std::vector<NodeID> getIDs (  );
		SDL_Renderer * currRenderer;
		Camera * currCamera;
		std::vector<NodeID> nodesID;
		std::vector<Node *> nodes;
		static Elements * singleton;
};

#endif // ELEMENTS_HPP
