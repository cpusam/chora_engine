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
#include "../include/collision.hpp"
#include "./Node.hpp"

typedef long int EntityID;

// devia estar em movable.hpp
enum Direction: unsigned int
{
	NONE_DIR=0x0,
	LEFT_DIR=0x00000001,
	RIGHT_DIR=0x00000002,
	UP_DIR=0x00000004,
	DOWN_DIR=0x00000008,
	CENTER_DIR = 0x000000FF,
	ALL_DIR=0xFFFFFFFF,
};

enum RelativePosition
{
	RELATIVE_WORLD=0x1,
	RELATIVE_ENTITY=0x2,
};

class Entity: public Node
{
	public:
		Entity();
		virtual ~Entity();
		
		//deprecated
		void setGroup ( std::string g );
		//deprecated
		std::string getGroup (  );

		void setDir ( Direction d );
		Direction getDir (  );
		Vect getMinVel (  );
		Vect getMaxVel (  );
		void setMinVel ( Vect minVel );
		void setMaxVel ( Vect maxVel );

		Vect getVelocity (  );
		float getVelocityX (  );
		float getVelocityY (  );
		void setVelocity ( Vect v );
		void setVelocityX ( float vx );
		void setVelocityY ( float vy );

		Vect getAccel (  );
		float getAccelX (  );
		float getAccelY (  );
		void setAccel ( Vect a );
		void setAccelX ( float ax );
		void setAccelY ( float ay );

		Vect getDamping (  );
		void setDamping ( Vect d );

		//adicona mais animação
		virtual void addAnim ( const Animation & anim, std::string name );
		Animation * getCurrAnim (  );
		//retorna a animação atual dada um nome qualquer, 
		//usada internamente por changeAnim
		virtual std::string getAnimName ( std::string animName );
		bool setCurrAnim ( std::string animName );//muda para uma animação já carregada de nome animName
		virtual void changeAnim ( std::string animName, bool reset=false );
		//atualiza a animação
		virtual void updateAnim (  );
		void setPosition ( Vect p ) override;
		Vect getPosition (  ) override;
		virtual void changeDir ( Direction d );
		
		// método que recebe mensagens de outros elementos
		// retorna true caso a mensagem tenha sigod usada; false caso contrario
		virtual bool receive ( Entity * sender, std::string mesg );

		TileMap * getLevel (  );
		void setLevel ( TileMap * map );

		bool isDir ( Direction d );

		bool isSolidSide ( std::string side, int i=-1 );
		//verifica se é sólido nos pontos laterias, ou no ponto lateral 'i'
		bool isSolidSide ( std::string side, SDL_Rect other, int i=-1 );
		// colisão
		void setSolids ( std::vector<int> solids );
		void setOneWayUp ( std::vector<int> oneWayUp );
		void setSlopeUp ( std::vector<int> slopeUp, std::map<int, std::vector<Vect> > angles );
		std::vector<int> getSlopeUpTiles (  );
		std::map<int, std::vector<Vect> > getSlopeUpAngles (  );
		std::vector<int> getSolids (  );
		std::vector<int> getOneWayUp (  );
		void addSolid ( int s );
		bool remSolid ( int s );
		//adiciona ou remove OneWay que colide em baixo e move pra cima
		void addOneWayUp ( int s );
		bool remOneWayUp ( int s );
		//adiciona ou remove rampa que colide em baixo e move pra cima
		//'a' e 'b' fazem uma angulação
		void addSlopeUp ( int s, Vect a, Vect b );
		bool remSlopeUp ( int s );
		bool isSolid ( Vect p );
		bool isSolidOneWayUp ( Vect p );
		bool isSolidSlopeUp ( Vect p, Vect * result=nullptr );
		bool oneWayUpCollision (  );
		bool slopeUpCollision (  );
		void setSlopeUpPivot ( const Vect & p );
		Vect getSlopeUpPivot (  );
		virtual bool isGround (  );
		virtual void setGround ( bool g );
		virtual void activeGround ( bool g );

		virtual bool moveToPosition (Vect position, float maxVel );
		virtual void setCountPath ( int count );
		virtual bool moveInPath ( Vect startPos, std::vector<Vect> & path, float maxVel, bool back );
		virtual bool collisionY (  );
		virtual bool collisionX (  );
		
		void setCollPos ( Vect p );
		Vect getCollPos (  );//retorna posição do collRect somada à posição do this
		Vect getCollCenter (  );
		//se relative == true retorna collRect sem somar 'position'
		SDL_Rect getCollRect ( RelativePosition relative=RELATIVE_WORLD );// retorna o retângulo de colisão em coordenadas do mundo
		void setCollRect ( SDL_Rect rect, int numPoints=0 );
		void setCollRect ( SDL_Rect rect, bool relative );
		SDL_Rect getView (  );
		std::vector<Vect> getSide ( std::string side, RelativePosition relative=RELATIVE_ENTITY );
		// define as laterais do corpo baseado num retangulo usando numPoints por lateral
		void setSides ( SDL_Rect rect, int numPoints );
		void drawSides ( SDL_Renderer * renderer, Camera * camera );
		
		//pega os hash para colisão, usado em alguns poucos casos
		std::vector<int> getTilesHash (  );
		//faz espelhar as animações
		void flipAnim ( bool hor, bool ver );
		//espelha a entidade
		void mirrorHor (  );

		//aplica impulso
		virtual void applyImpulse ( Vect impulse );
		virtual void applyImpulseX ( float ix );
		virtual void applyImpulseY ( float iy );
		//move com velocidade limitada por maxVel
		// add = adiciona à velocidade antes junto com a aceleração
		virtual void moveX ( float add = 0 );
		virtual void moveY ( float add = 0 );

		virtual std::string getStateString (  );
		virtual std::string to_json ();

		void input ( SDL_Event & event ) override;
		void draw ( SDL_Renderer * renderer, Camera * camera ) override;
		int update ( double delta=0 ) override;

		virtual void updatePosition (  )
		{
			if (beforePos.x != position.x)
				beforePos.x = position.x;
			if (beforePos.y != position.y)
				beforePos.y = position.y;
		}
	protected:
		Vect beforePos;
		//definido apenas uma vez, depois volta para false
		bool ground;//se é chão definido por setGround
		bool noground;//se true isGround vai retornar sempre false
		int layer;//camada de desenho, quanto meno primeiro desenha
		Vect damping;//desaceleração (de 0 ao 1)
		Vect maxVel, minVel;
		TileMap * level;//mapa de tiles sólidos e vázios, é o mapa de colisão
		Direction dir;
		Vect velocity;
		Vect acc;

		int collPoints;//numero de pontos de colisão
		SDL_Rect collRect;//retangulo de colisão
		SDL_Rect view;//rect que cobre toda a imagem de entity
		SDL_Texture * texture;
		Animation * currAnim;
		std::map<std::string, Animation> anim;
		std::string name;
		std::string group; //grupo desta entidade

		
		//tile one way sólido em cima
		std::vector<int> upSolid;
		std::vector<int> upSolidSlope;
		std::vector<int> solid;//tiles sólidos
		//angulos dos slopes
		std::map<int, std::vector<Vect> > upSolidSlopeAngles;
		//pivo do slope up
		Vect slopeUpPivot;
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
