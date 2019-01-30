#ifndef CHORA_NODE_HPP
#define CHORA_NODE_HPP

#include <string>
#include "../include/camera.hpp"
#include "../include/statemachine.hpp"

typedef long int NodeID;
typedef long int NodeType;
typedef int NodeGroups;

//era pra ser uma classe template
//nó de arvore
class Node: public StateMachine
{
	public:
		Node ( std::string name="Node", int initState=-1, NodeGroups groups=0, NodeType type=0 );
		virtual ~Node (  );

		virtual bool receive ( Node * sender, std::string mesg );
		virtual Node * clone (  );

		virtual void addGroups ( NodeGroups g );

		virtual void draw ( SDL_Renderer * renderer, Camera * camera );

		//tem de colocar essas funções quando forem usadas engines diferentes
		virtual Vect getPosition (  );
		virtual float getPositionX (  );
		virtual float getPositionY (  );

		virtual void setPosition ( Vect p );
		virtual void setPositionX ( float px );
		virtual void setPositionY ( float py );

		//escala
		virtual Vect getScale (  );
		virtual float getScaleX (  );
		virtual float getScaleY (  );

		virtual bool isVisible (  );
		//verifica se os bits de g estão em groups
		virtual bool insideGroup ( NodeGroups g );
		NodeGroups getGroups (  );

		NodeID getID (  );
		//camada de desenho
		int getLayer (  );

		virtual std::string getName (  );
		virtual Node * getParent (  );
		virtual NodeType getType (  ); 

		//ativa ou desativa o bit do grupo
		//g é 0 para desativar o grupo ou 1 para ativar
		//pos vai de 0 ao 31
		virtual void setGroup ( bool g, int pos );
		
		virtual void show ( bool s );
		void setLayer ( int l );

		virtual void setName ( std::string name );
		virtual std::vector<Node *> const & getChildren (  ); 
		//adiciona um filho
		virtual void addChild ( Node * child );
		//remove um filho 
		virtual void removeChild ( Node * child );
		//atualiza a posição dos filhos
		virtual void updateChildrenPosition (  );
		virtual void setParent ( Node * p );
		virtual void setType ( NodeType t );

	protected:
		bool visible;
		int layer;
		Node * parent;
		std::vector<Node *> children;

		SDL_Rect dimension;
		Vect position;
		Vect scale;
		std::string name;
		NodeType type;
		NodeGroups groups;

	private:
		static NodeID countID;
		NodeID id;
};

#endif