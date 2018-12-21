#ifndef CHORA_NODE_HPP
#define CHORA_NODE_HPP

typedef long int NodeID;
typedef int NodeGroups;

//era pra ser uma classe template
//nó de arvore
class Node: public StateMachine
{
	public:
		Node (  );
		virtual ~Node (  );

		virtual bool receive ( Node * sender, std::string mesg );

		virtual void addGroups ( NodeGroups g );
		virtual void draw ( SDL_Renderer * renderer, Camera * camera );

		virtual bool isVisible (  );
		//verifica se os bits de g estão em groups
		virtual bool isInsideGroup ( NodeGroups g );
		NodeGroups getGroups (  );

		NodeID getID (  );
		//camada de desenho
		int getLayer (  );

		virtual std::string getName (  );
		virtual Node * getNextA (  );
		virtual Node * getNextB (  );
		virtual Node * getParent (  );

		//ativa ou desativa o bit do grupo
		//g é 0 para desativar o grupo ou 1 para ativar
		//pos vai de 0 ao 31
		virtual void setGroup ( bool g, int pos );
		
		virtual void show ( bool s );
		void setLayer ( int l );

		virtual void setName ( std::string name );
		virtual void setNextA ( Node * n );
		virtual void setNextB ( Node * n );
		virtual void setParent ( Node * p );

	protected:
		bool visible;
		int layer;
		Node * parent;
		Node * nextA;
		Node * nextB;
		std::string name;
		NodeGroups groups;

	private:
		static NodeID countID;
		NodeID id;
};

#endif