/*
	IMPORTANTE: para o Entity.changeDir ter o efeito certo a imagem dos sprites devem estar viradas para esquerda
*/
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <map>
#include <vector>
#include "../Chora.hpp"

// devia estar em movable.hpp
enum Direction: unsigned int
{
	NONE_DIR=0x0,
	LEFT_DIR=0x00000001,
	RIGHT_DIR=0x00000002,
	UP_DIR=0x00000004,
	DOWN_DIR=0x00000008,
	ALL_DIR=0xFFFFFFFF,
};

class Entity: public StateMachine, public Movable
{

	public:
		Entity();
		virtual ~Entity();

		int getLayer (  );
		void setLayer ( int layer );
		void setName ( std::string n );
		std::string getName (  );
		void setGroup ( std::string g );
		std::string getGroup (  );
		int getId (  );
		void show ( bool s );
		bool isVisible (  );
		void setDir ( Direction d );
		Direction getDir (  );
		Animation * getCurrAnim (  );
		bool setCurrAnim ( std::string animName );//muda para uma animação já carregada de nome animName
		virtual void setPosition ( Vect p );
		virtual Vect getPosition (  );
		void setTopLadderTile ( int t );
		int getTopLadderTile (  );
		virtual void changeDir ( Direction d );

		// método que recebe mensagens de outros elementos
		// retorna true caso a mensagem tenha sigod usada; false caso contrario
		virtual bool receive ( Entity * sender, std::string mesg );

		TileMap * getLevel (  );
		void setLevel ( TileMap * map );

		bool isDir ( Direction d );

		bool isSolidSide ( std::string side, int i=-1 );

		// colisão
		std::vector<int>& getSolids (  );
		void addSolid ( int s );
		bool remSolid ( int s );
		void addOneWayUp ( int s );
		bool remOneWayUp ( int s );
		bool isSolid ( Vect p );
		bool isGround (  );
		bool isLadder (  );
		bool isTopLadder (  );
		void catchLadder (  );
		bool oneWayUpCollision();
		bool collisionVer (  );
		bool collisionHor (  );
		
		Vect getCollCenter (  );
		SDL_Rect getCollRect (  );// retorna o retângulo de colisão em coordenadas do mundo
		SDL_Rect getView (  );
		// define as laterais do corpo baseado num retangulo usando numPoints por lateral
		void setSides ( SDL_Rect rect, int numPoints );
		void drawSides ( SDL_Renderer * renderer, Camera * camera );

		virtual std::string getStateString (  );
		virtual std::string to_json ();

		virtual void input ( SDL_Event & event );
		virtual void draw ( SDL_Renderer * renderer, Camera * camera );
		virtual int update (  );

	protected:
		int layer;//camada de desenho, quanto meno primeiro desenha
		TileMap * level;//mapa de tiles sólidos e vázios, é o mapa de colisão
		Direction dir;

		int collPoints;//numero de pontos de colisão
		int ladderTile;//tile da escada que não é o final
		int topLadderTile;//tile ponta de escada
		int topTileSize; // altura para colisão do tile da ponta da escada
		SDL_Rect collRect;//retangulo de colisão
		SDL_Rect view;//rect que cobre toda a imagem de entity
		SDL_Texture * texture;
		Animation * currAnim;
		std::map<std::string, Animation> anim;
		std::string name;
		std::string group; //grupo desta entidade

		std::vector<int> upSolid;//tile one way sólido em cima
		std::vector<int> solid;//tiles sólidos
		// cada lado tem collPoints de vertices de colisão
		std::vector<Vect> rightSide,leftSide;//lateral direita e esquerda do corpo da entidade
		std::vector<Vect> upSide, downSide;//leteral de cima e em baixo do corpo da entidade

	private:
		static int countID;
		int id;
		bool visible;
};

#endif // ENTITY_HPP
