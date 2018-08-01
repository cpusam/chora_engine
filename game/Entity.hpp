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
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <map>
#include <vector>

#include "../platform/tilemap.hpp"
#include "../include/movable.hpp"
#include "../include/statemachine.hpp"

typedef long int EntityID;

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

enum RelativePosition
{
	RELATIVE_WORLD=0x1,
	RELATIVE_ENTITY=0x2,
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
		EntityID getId (  );
		void show ( bool s );
		bool isVisible (  );
		void setDir ( Direction d );
		Direction getDir (  );
		Vect getMinVel (  );
		Vect getMaxVel (  );
		void setMinVel ( Vect maxVel );
		void setMaxVel ( Vect maxVel );
		Vect getDamping (  );
		void setDamping ( Vect d );


		Animation * getCurrAnim (  );
		bool setCurrAnim ( std::string animName );//muda para uma animação já carregada de nome animName
		virtual void changeAnim ( std::string animName, bool reset=false );
		//atualiza a animação
		virtual void updateAnim (  );
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
		bool isLadder (  );
		bool isTopLadder (  );
		void catchLadder (  );
		bool oneWayUpCollision (  );
		virtual bool isGround (  );

		bool moveToPosition (Vect pos, float maxVel );
		void setCountPath ( int count );
		bool moveInPath ( Vect startPos, std::vector<Vect> & path, float maxVel, bool back );
		virtual bool collisionVer (  );
		virtual bool collisionHor (  );
		
		Vect getCollPos (  );//retorna posição do collRect somada à posição do this
		Vect getCollCenter (  );
		//se relative == true retorna collRect sem somar 'pos'
		SDL_Rect getCollRect ( RelativePosition relative=RELATIVE_WORLD );// retorna o retângulo de colisão em coordenadas do mundo
		void setCollRect ( SDL_Rect rect, int numPoints=0 );
		SDL_Rect getView (  );
		std::vector<Vect> getSide ( std::string side, RelativePosition relative=RELATIVE_ENTITY );
		// define as laterais do corpo baseado num retangulo usando numPoints por lateral
		void setSides ( SDL_Rect rect, int numPoints );
		void drawSides ( SDL_Renderer * renderer, Camera * camera );

		//aplica impulso
		void applyImpulse ( Vect impulse );
		//move com velocidade limitada por maxVel
		// add = adiciona à velocidade antes junto com a aceleração
		virtual void moveX ( float add = 0 );
		virtual void moveY ( float add = 0 );

		virtual std::string getStateString (  );
		virtual std::string to_json ();

		virtual void input ( SDL_Event & event );
		virtual void draw ( SDL_Renderer * renderer, Camera * camera );
		virtual int update (  );

	protected:
		Vect damping;//desaceleração (de 0 ao 1)
		Vect maxVel, minVel;
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
		static EntityID countID;
		EntityID id;
		bool visible;
		bool changeBack;
		int countPath;
};

#endif // ENTITY_HPP
