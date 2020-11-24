#include "Entity.hpp"
#include "Exception.hpp"
#include "SDL_gfx/SDL_framerate.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

EntityID Entity::countID = 1;

Entity::Entity()
{
	layer = 0;
	id = countID;
	countID++;
	std::cout<<"New Entity ID="<<id<<std::endl;
	dir = NONE_DIR;
	visible = true;

	ground = false;
	noground = false;
	level = nullptr;
	collPoints = 3;
	texture = nullptr;
	currAnim = nullptr;
	name = "";
	countPath = 0;
	
	//coloca a velocidade máxima... ao máximo
	maxVel.set(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	//minimo de desaceleração
	damping.set(0.0,0.0);
	//
	setCollRect((SDL_Rect){0,0,3,3}, collPoints);
	slopeUpPivot.set(collRect.w/2, collRect.h);
}

Entity::~Entity()
{
	std::cout<<"Deletando "<<getName()<<" id="<<getID()<<std::endl;
}

void Entity::setGroup ( std::string g )
{
	group = g;
}

std::string Entity::getGroup (  )
{
	return group;
}

void Entity::setDir ( Direction d )
{
	dir = d;
}

Direction Entity::getDir (  )
{
	return dir;
}

Vect Entity::getMinVel (  )
{
	return minVel;
}

Vect Entity::getMaxVel (  )
{
	return maxVel;
}

void Entity::setMinVel ( Vect minVel )
{
	this->minVel = minVel;
}

void Entity::setMaxVel ( Vect maxVel )
{
	this->maxVel = maxVel;
}

Vect Entity::getVelocity (  )
{
	return velocity;
}

float Entity::getVelocityX (  )
{
	return velocity.x;
}

float Entity::getVelocityY (  )
{
	return velocity.y;
}

void Entity::setVelocity ( Vect v )
{
	velocity = v;
}

void Entity::setVelocityX ( float vx )
{
	velocity.x = vx;
}

void Entity::setVelocityY ( float vy )
{
	velocity.y = vy;
}

Vect Entity::getAccel (  )
{
	return acc;
}

float Entity::getAccelX (  )
{
	return acc.x;
}

float Entity::getAccelY (  )
{
	return acc.y;
}

void Entity::setAccel ( Vect a )
{
	acc = a;
}

void Entity::setAccelX ( float ax )
{
	acc.x = ax;
}

void Entity::setAccelY ( float ay )
{
	acc.y = ay;
}

Vect Entity::getDamping (  )
{
	return damping;
}

void Entity::setDamping ( Vect d )
{
	damping = d;
}

void Entity::addAnim ( const Animation & anim, std::string name )
{
	this->anim[name] = anim;
}

Animation * Entity::getCurrAnim (  )
{
	return currAnim;
}

std::string Entity::getAnimName ( std::string animName )
{
	return animName;
}

bool Entity::setCurrAnim ( std::string animName )
{
	std::map<std::string, Animation>::const_iterator it = anim.find(animName);

	if (it != anim.end())
	{
		currAnim = &anim[animName];
		return true;
	}

	return false;
}

void Entity::changeAnim ( std::string animName, bool reset )
{
	if (anim.size() > 0)
	{
		animName = getAnimName(animName);
		if (anim.find(animName) != anim.end())
		{
			currAnim = &anim[animName];
		}
		else
		{
			currAnim = nullptr;
			std::cout<<"Entity->"<<getName()<<"::Não encontrou uma animação chamada "<<animName<<std::endl;
		}
	}

	if (reset && currAnim)
		currAnim->reset();
}

void Entity::updateAnim (  )
{
	if (currAnim)
		currAnim->update();
}

Vect Entity::getPosition (  )
{
	return position;
}

void Entity::setPosition ( Vect p )
{
	position = p;
	updatePosition();
}

void Entity::changeDir ( Direction d )
{
	bool changeH = true, changeV = true;

	if (dir == d)
		return;


	changeH = (d & RIGHT_DIR)||(d & LEFT_DIR);
	changeV = (d & UP_DIR)||(d & DOWN_DIR);

	for (std::map<std::string,Animation>::iterator it = anim.begin(); it != anim.end(); it++)
		it->second.flip(changeH, changeV);

	dir = d;
}

bool Entity::receive ( Entity * sender, std::string mesg )
{
	return false;
}

TileMap * Entity::getLevel (  )
{
	return level;
}

void Entity::setLevel ( TileMap * level )
{
	this->level = level;
}

void Entity::setCollPos ( Vect p )
{
	updatePosition();
	position.x = p.x - collRect.x;
	position.y = p.y - collRect.y;
}

Vect Entity::getCollPos (  )
{
	return Vect(position.x + collRect.x, position.y + collRect.y);
}

Vect Entity::getCollCenter (  )
{
	return Vect(int(position.x)+collRect.x+collRect.w/2,int(position.y)+collRect.y+collRect.h/2);
}

SDL_Rect Entity::getCollRect ( RelativePosition relative )
{
	if (relative == RELATIVE_WORLD)
		return (SDL_Rect){int(position.x) + collRect.x, int(position.y) + collRect.y, collRect.w,collRect.h};
	
	return collRect;
}

void Entity::setCollRect ( SDL_Rect rect, int numPoints )
{
	collRect = rect;
	if (numPoints > 2)
		setSides(rect, numPoints);
}

SDL_Rect Entity::getView (  )
{
	return (SDL_Rect){int(position.x) + view.x, int(position.y) + view.y, view.w,view.h};
}

bool Entity::isDir ( Direction d )
{
	return d & dir;
}

bool Entity::isSolidSide ( std::string side, int i )
{
	std::vector<Vect> vet;
	if (side == "up")
		vet = upSide;
	else if (side == "down")
		vet = downSide;
	else if (side == "right")
		vet = rightSide;
	else if (side == "left")
		vet = leftSide;
	else
	{
		std::cout<<"Error "<<side<<" lado não identificado\n";
		return false;
	}

	if (i >= 0)
	{
		if (i < int(vet.size()))
		{
			Vect p = vet.at(i);

			if (side == "up")
				p.y -= 1;
			else if (side == "down")
				p.y += 1;
			else if (side == "right")
				p.x += 1;
			else if (side == "left")
				p.x -= 1;

			return isSolid(p + position);
		}

		return false;
	}

	for (auto & v: vet)
	{
		if (side == "up")
			v.y -= 1;
		else if (side == "down")
			v.y += 1;
		else if (side == "right")
			v.x += 1;
		else if (side == "left")
			v.x -= 1;

		if (isSolid(Vect::add(v, position)))
			return true;
	}

	return false;
}


bool Entity::isSolidSide ( std::string side, SDL_Rect other, int i )
{
	std::vector<Vect> vet;
	if (side == "up")
		vet = upSide;
	else if (side == "down")
		vet = downSide;
	else if (side == "right")
		vet = rightSide;
	else if (side == "left")
		vet = leftSide;
	else
	{
		std::cout<<"Error "<<side<<" lado não identificado\n";
		return false;
	}

	if (i >= 0)
	{
		if (i < int(vet.size()))
		{
			Vect p = vet.at(i);

			if (side == "up")
				p.y -= 1;
			else if (side == "down")
				p.y += 1;
			else if (side == "right")
				p.x += 1;
			else if (side == "left")
				p.x -= 1;

			return pointbox(Vect::add(p, position), other);
		}

		return false;
	}

	for (auto & v: vet)
	{
		if (side == "up")
			v.y -= 1;
		else if (side == "down")
			v.y += 1;
		else if (side == "right")
			v.x += 1;
		else if (side == "left")
			v.x -= 1;

		if (pointbox(Vect::add(v, position), other))
			return true;
	}

	return false;
}

void Entity::setSolids ( std::vector<int> solids )
{
	solid = solids;
}

void Entity::setOneWayUp ( std::vector<int> oneWayUp )
{
	upSolid = oneWayUp;
}

void Entity::setSlopeUp ( std::vector<int> slopeUp, std::map<int, std::vector<Vect> > angles )
{
	upSolidSlope = slopeUp;
	upSolidSlopeAngles = angles;
}

std::vector<int> Entity::getSlopeUpTiles (  )
{
	return upSolidSlope;
}

std::map<int, std::vector<Vect> > Entity::getSlopeUpAngles (  )
{
	return upSolidSlopeAngles;
}

std::vector<int> Entity::getSolids (  )
{
	return solid;
}

std::vector<int> Entity::getOneWayUp (  )
{
	return upSolid;
}

void Entity::addSolid ( int s )
{
	for (auto i: solid)
		if (i == s)
			return;
	solid.push_back(s);
}

bool Entity::remSolid ( int s )
{
	unsigned int bef = solid.size();
	std::vector<int> aux;
	for (auto i: solid)
		if (i != s)
		{
			aux.push_back(i);
		}

	solid = aux;
	return bef != solid.size();
}

void Entity::addOneWayUp ( int s )
{
	upSolid.push_back(s);
}

bool Entity::remOneWayUp ( int s )
{
	unsigned int bef = upSolid.size();
	std::vector<int> aux;
	for (auto i: upSolid)
		if (i != s)
		{
			aux.push_back(i);
		}

	upSolid = aux;
	return bef != upSolid.size();
}

void Entity::addSlopeUp ( int s, Vect a, Vect b )
{
	for (auto i: upSolidSlope)
		if (i == s)
			return;
	upSolidSlope.push_back(s);
	upSolidSlopeAngles[s].push_back(a);
	upSolidSlopeAngles[s].push_back(b);
}

bool Entity::remSlopeUp ( int s )
{
	unsigned int bef = upSolidSlope.size();
	std::vector<int> aux;
	for (auto i: upSolidSlope)
		if (i != s)
			aux.push_back(i);

	upSolidSlope = aux;
	upSolidSlopeAngles.erase(s);
	return bef != upSolid.size();
}

bool Entity::isSolid ( Vect p )
{
	if (!level)
	{
		std::cerr<<"Entity::isSolid level é nulo p=";
		p.print();
		return false;
	}

	int tile = level->getTile(p.x, p.y);
	if (tile < 0)
		return false;
	
	return std::find(solid.begin(), solid.end(), tile) != solid.end();
}

bool Entity::isSolidOneWayUp ( Vect p )
{
	if (!level)
	{
		std::cerr<<"Entity::isSolid level é nulo p=";
		p.print();
		return false;
	}

	int tile = level->getTile(p.x, p.y);
	if (tile < 0)
		return false;
	
	return std::find(upSolid.begin(), upSolid.end(), tile) != upSolid.end();
}

bool Entity::isSolidSlopeUp ( Vect p, Vect * result )
{
	if (!level)
	{
		std::cerr<<"Entity::isSolidSlope level é nulo p=";
		p.print();
		return false;
	}

	
	int tile = level->getTile(p.x, p.y);
	if (tile < 0)
	{
		p = getCollCenter();
		tile = level->getTile(p.x, p.y);
		if (tile < 0)
			return false;
		p.x = getCollPos().x + slopeUpPivot.x;
		p.y = getCollPos().y + slopeUpPivot.y;
	}

	if (upSolidSlopeAngles.find(tile) != upSolidSlopeAngles.end())
	{
		int tilesize = level->getTilesize();
		Vect center[2] = {Vect(p.x, p.y - slopeUpPivot.y), Vect(p.x, p.y + 5)};
		//Vect center[2] = {Vect(p.x, p.y), Vect(p.x, p.y + (slopeUpPivot.y - (p.y - getCollPos().y)))};
		Vect slopePos(int(p.x / tilesize) * tilesize, int(p.y / tilesize) * tilesize);
		Vect aux[2] = {upSolidSlopeAngles[tile][0], upSolidSlopeAngles[tile][1]};
		return lineIntersects(Vect::add(slopePos, aux[0]), Vect::add(slopePos, aux[1]), center[0], center[1], result);
	}

	return false;
}

bool Entity::isGround (  )
{
	//retorna sempre false sem desativar
	if (noground)
	{
		return false;
	}

	//ativa o chão somente uma vez
	if (ground)
	{
		ground = false;
		return true;
	}

	if (!level)
	{
		std::cerr<<"Entity::isGround level é nulo\n";
		return false;
	}

	//setSides(collRect, collPoints);

	float posX = position.x + collRect.x;
	float posY = position.y + collRect.y + collRect.h;
	
	{
		Vect p;
		p.x = posX;
		p.y = posY;
		if (isSolidSlopeUp(p))
		{
			return true;
		}

		p.x = posX + collRect.w;
		p.y = posY;
		if (isSolidSlopeUp(p))
		{
			return true;
		}
	}

	if (velocity.y == 0 && upSolid.size() != 0)
	{
		Vect q;
		for (auto p: downSide)
		{
			p.x += position.x;
			p.y += position.y;
			q.set(p.x, p.y-1);
			p.y += 1;
			if (isSolidOneWayUp(p) && isSolidOneWayUp(q) == false)
				return true;
		}
	}

	for (auto p: downSide)
	{
		p.x += position.x;
		p.y += position.y;
		p.y += 1; // 1 pixel abaixo
		// verifica se é sólido
		if (isSolid(p))
			return true;
	}
/*
	// meio da lateral de baixo do retangulo de colisão (collRect)
	int x1 = position.x + collRect.x;
	int y1 = position.y + collRect.y + collRect.h;
	// +1 para detectar o tile abaixo dele
	y1 = y1 + 1;

	int x2 = position.x + collRect.x + collRect.w;
	int y2 = y1;

	if (topLadderTile >= 0)
	{
		int levelY = (y1/level->getTilesize())*level->getTilesize();
		//verifica se é o topo da escada na parte de baixo (downSide)
		if ((y1 - levelY) <= topTileSize && (level->getTile(x1,y1) == topLadderTile || level->getTile(x2,y2) == topLadderTile))
			if (velocity.y == 0)
				return true;
	}
*/
	return false;
}

void Entity::setGround ( bool g )
{	
	ground = g;
}

void Entity::activeGround ( bool g )
{
	noground = g;
}

bool Entity::moveToPosition ( Vect position, float maxVelNow )
{
	Vect diff(position.x - this->position.x, position.y - this->position.y);

	float seconds = FPSManager::instance()->getDeltaSeconds();

	diff.normalize();
	velocity.x = maxVelNow * diff.x * seconds;
	velocity.y = maxVelNow * diff.y * seconds;

	updatePosition();
	this->position.x += velocity.x;
	this->position.y += velocity.y;
	

	bool changeX = false, changeY = false;
	if (velocity.x < 0)
		changeX = int(this->position.x) < int(position.x);
	else if (velocity.x > 0)
		changeX = int(this->position.x) > int(position.x);
	
	if (velocity.y < 0)
		changeY = int(this->position.y) < int(position.y);
	else if (velocity.y > 0)
		changeY = int(this->position.y) > int(position.y);

	return changeX && changeY;
}

void Entity::flipAnim ( bool hor, bool ver )
{
	for (auto & it: anim)
		it.second.flip(hor, ver);
}

void Entity::mirrorHor (  ) {
	if (getDir() == Direction::LEFT_DIR) {
		changeDir(Direction::RIGHT_DIR);
	}
	else if (getDir() == Direction::RIGHT_DIR) {
		changeDir(Direction::LEFT_DIR);
	}
}

//move num caminho realtivo à posição do corpo
// back é pra retornar quando chegar ao fim
void Entity::setCountPath ( int count)
{
	countPath = 0;
}

bool Entity::moveInPath (Vect position, std::vector<Vect> & path, float maxVel, bool back )
{
	if (path.size() == 0 || countPath >= int(path.size()))
		return false;

	Vect point = Vect(path[countPath].x + position.x, path[countPath].y + position.y);

	if (countPath > -1 && moveToPosition(point, maxVel))
	{
		if (changeBack == false)
		{
			countPath++;
			if (countPath >= int(path.size()))
			{
				if (back)
				{
					changeBack = true;
					countPath = path.size() - 1;
				}
			}
		}
		else
		{
			countPath--;
			if (countPath < 0)
			{
				countPath = 0;
				if (back)
					changeBack = false;
			}
		}

		return true;
	}

	return false;
}


bool Entity::oneWayUpCollision ()
{
	if (!level)
	{
		std::cerr<<"Entity::Erro level é nulo\n";
		return false;
	}

	if (velocity.y <= 0)
		return false;

	Vect collPos = position;
	for (auto p: downSide)
	{
		Vect before(Vect::add(collPos, p));
		Vect after(before);
		before.y = beforePos.y;

		if (isSolidOneWayUp(before) == false && isSolidOneWayUp(after))
		{
			int y = (int(after.y) / level->getTilesize()) * level->getTilesize();
			after.y = y - collRect.h - 1;
			after.x = collPos.x;
			updatePosition();
			setCollPos(after);
			return true;
		}
	}

	return false;
}

bool Entity::slopeUpCollision (  )
{
	if (velocity.y <= 0)
		return false;
	
	Vect result;
	float centerX = position.x + collRect.x + slopeUpPivot.x;
	for (auto p: leftSide)
	{
		p.x = centerX;
		p.y += position.y;
	
		if (isSolidSlopeUp(p, &result))
		{
			//move pra fora do slope
			result.y -= slopeUpPivot.y;
			updatePosition();
			//result.x = position.x + collRect.x;
			setCollPos(result);
			return true;
		}
	}

	return false;
}

void Entity::setSlopeUpPivot ( const Vect & p )
{
	slopeUpPivot = p;
}

Vect Entity::getSlopeUpPivot (  )
{
	return slopeUpPivot;
}

bool Entity::collisionY (  )
{
	if (!level)
	{
		std::cerr<<"Entity::"+name+" collisionY level map é nulo"<<std::endl;
		return false;
	}
	if (velocity.y == 0)
		return false;
	
	bool ret = false;
	float beforeX = getCollPos().x;
	//setSides(collRect, collPoints);

	if (velocity.y < 0)
	{
		//colisão acima da cabeça e embaixo do tile
		for (auto p: upSide)
		{
			p.x += position.x;
			p.y += position.y;
			if (isSolid(p))
			{
				int y = ((int(p.y) / level->getTilesize()) + 1) * level->getTilesize();
				updatePosition();
				//tem que arrendondar o p.y para evitar bugs
				//esse 5 não era pra estar aqui!
				setCollPos(Vect(beforeX, p.y + collRect.y + (y - p.y)));
				//level->setTile(beforeX, getCollPos().y, '.');
				ret = true;
				break;
			}
		}
	}
	else
	{
		//colisão com a parte de baixo de entity
		for (auto p: downSide)
		{
			p.x += position.x;
			p.y += position.y;
			if (isSolid(p))
			{
				int y = (int(p.y) / level->getTilesize())*level->getTilesize();
				updatePosition();
				setCollPos(Vect(beforeX, y - collRect.h - 1));
				ret = true;
				break;
			}
		}

		//tenta colidir com algum tile one way, 
		//colidir em cima do tile e embaixo de Entity
		if (oneWayUpCollision())
			ret = true;
		
		//colisão com os slopes
		//SlopeUP
		Vect result;
		for (Vect p: leftSide)
		{
			p.x += position.x;
			p.y += position.y;
			if (isSolidSlopeUp(p, &result))
			{
				p.y = result.y - (collRect.y + collRect.h);
				updatePosition();
				setCollPos(p);
				ret = true;
				break;
			}
		}
		/*
		// colisão com a ponta da escada
		// aqui é colisão "oneway" do lado de cima
		if (!ret && topLadderTile > -1)
			for (auto p: downSide)
			{
				p = p + position;
				p.y += velocity.y;
				p.y = round(p.y);
				int x = p.x;
				int y = p.y;
				int levelY = (y / level->getTilesize())*level->getTilesize();
				if (y - levelY < topTileSize)
					if (level->getTile(x,y) == topLadderTile)
					{
						p.y = p.y - (collRect.y + collRect.h) + (levelY - int(p.y));
						p.y = floor(position.y) - 1;
						setCollPos(p);
						setSides(collRect,collPoints);
						ret = true;
						break;
					}
			}
		*/
	}

	return ret;
}

bool Entity::collisionX (  )
{
	if (!level)
	{
		std::cerr<<"Entity::"+name+" collisionX level map é nulo"<<std::endl;
		return false;
	}

	if (velocity.x == 0)
		return false;
	bool ret = false;
	float beforeY = getCollPos().y;
	//setSides(collRect, collPoints);

	if (velocity.x < 0)
	{
		for (auto p: leftSide)
		{
			p.x += position.x;
			p.y += position.y;
			if (isSolid(p))
			{
				int x = (int(p.x) / level->getTilesize() + 1) * level->getTilesize();
				p.x = x;
				updatePosition();
				setCollPos(Vect(p.x, beforeY));
				ret = true;
				break;
			}
		}
	}
	else
	{
		for (auto p: rightSide)
		{
			p.x += position.x;
			p.y += position.y;
			if (isSolid(p))
			{
				int x = (int(p.x) / level->getTilesize()) * level->getTilesize();
				p.x = int(x - collRect.w - 1);
				updatePosition();
				setCollPos(Vect(p.x,beforeY));
				ret = true;
				break;
			}
		}
	}

	return ret;
}

std::vector<Vect> Entity::getSide ( std::string side, RelativePosition relative )
{
	std::vector<Vect> ret, ref;

	setSides(collRect, collPoints);
	if (side == "left")
	{
		ref = leftSide;
	}
	else if (side == "right")
	{
		ref = rightSide;
	}
	else if (side == "down")
	{
		ref = downSide;
	}
	else if (side == "up")
	{
		ref = upSide;
	}
	else
	{
		throw Exception("Erro: lado "+side+" não encontrado");
	}

	if (relative == RELATIVE_WORLD)
	{
		for (auto it: ref)
		{
			ret.push_back(Vect::add(position, it));
		}
	}
	else
	{
		ret = ref;
	}

	return ret;
}

void Entity::setSides ( SDL_Rect rect, int numPoints )
{
	float size, x, y;

	if (numPoints <= 2)
		throw Exception("Entity::"+name+" setSides numPoints <= 2");

	collPoints = numPoints;

	// limpa tudo
	rightSide.clear();
	leftSide.clear();
	upSide.clear();
	downSide.clear();

	size = float(rect.h) / float(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado direito
		x = rect.x + rect.w;
		y = rect.y + float(i) * size;
		rightSide.push_back(Vect(x,y));

		// define o lado esquerdo
		x = rect.x;
		//y = i * size;
		leftSide.push_back(Vect(x,y));
	}

	size = float(rect.w) / float(numPoints - 1);
	for (int i = 0; i < numPoints; i++)
	{
		// define o lado superior
		x = rect.x + float(i) * size;
		y = rect.y;
		upSide.push_back(Vect(x,y));

		// define o lado inferior
		//x = 0;
		y = rect.y + rect.h;
		downSide.push_back(Vect(x,y));
	}

}

void Entity::drawSides ( SDL_Renderer * renderer, Camera * camera )
{
	SDL_Color color = {255,0,0,255};
	SDL_Rect rect = {0,0,5,5};

	for (auto p: rightSide)
	{
		rect.x = p.x + position.x;
		rect.y = p.y + position.y;
		fillRect(renderer, camera, color, rect);
	}

	for (auto p: leftSide)
	{
		rect.x = p.x + position.x;
		rect.y = p.y + position.y;
		fillRect(renderer, camera, color, rect);
	}

	color.r = 255;
	color.g = 255;
	for (auto p: downSide)
	{
		rect.x = p.x + position.x;
		rect.y = p.y + position.y;
		fillRect(renderer, camera, color, rect);
	}

	// centro
	rect.x = position.x + collRect.x + collRect.w/2;
	rect.y = position.y + collRect.y + collRect.h/2;
	fillRect(renderer, camera, color, rect);

}

std::vector<int> Entity::getTilesHash (  )
{
	std::vector<int> ret;

	if (!level)
	{
		std::cout<<"Entity::Erro level é nulo\n";
		return ret;
	}

	int tilesize = level->getTilesize();
	int width = level->getWidth();

	
	if (leftSide.size())
	{
		int x = (leftSide[0].x + position.x) / tilesize;
		for (int i = leftSide.size() - 1; i > -1; i--)
		{
			int y = (leftSide[i].y + position.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	
	if (rightSide.size())
	{
		int x = (rightSide[0].x + position.x) / tilesize;
		for (int i = int(rightSide.size() - 1); i > -1; i--)
		{
			int y = (rightSide[i].y + position.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}
	
	if (downSide.size())
	{
		int y = (downSide[0].y + position.y) / tilesize;
		for (int i = int(downSide.size() - 1); i > -1; i--)
		{
			int x = (downSide[i].y + position.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	if (upSide.size())
	{
		int y = (upSide[0].y + position.y) / tilesize;
		for (int i = int(upSide.size() - 1); i > -1; i--)
		{
			int x = (upSide[i].y + position.y) / tilesize;
			ret.push_back(y * width + x);
		}
	}

	return ret;
}

void Entity::applyImpulse ( Vect impulse )
{
	velocity = Vect::add(velocity, impulse);
}

void Entity::applyImpulseX ( float ix )
{
	velocity.x += ix;
}

void Entity::applyImpulseY ( float iy )
{
	velocity.y += iy;
}

void Entity::moveX ( float add )
{
	bool damped = false;
	//aplica aceleração
	add = double(acc.x + add);//* double(FPSManager::instance()->getDeltaSeconds())
	velocity.x += add;
	
	//se não está aplicando desaceleração e a aceleração for zero...
	if (add == 0)
	{
		//então aplique a desaceleração
		velocity.x -= velocity.x * damping.x;
		damped = true;
	}
	
	if (velocity.x > minVel.x && velocity.x > maxVel.x)
		velocity.x = maxVel.x;
	else if (velocity.x < -minVel.x && velocity.x < -maxVel.x)
		velocity.x = -maxVel.x;
	//está abaixo do intervalo
	else if (!damped && add == 0)
	{
		velocity.x -= velocity.x * damping.x;
	}

	updatePosition();
	/*
		NOTA IMPORTANTE:
		pra manter o movimento fluido e constante, tem de multiplicar
		a velocidade pelo delta apenas quando vai somar na posição
	*/
	position.x += velocity.x * double(FPSManager::instance()->getDeltaSeconds());
}

void Entity::moveY ( float add )
{
	bool damped = false;
	//aplica aceleração
	add = double(acc.y + add);// * double(FPSManager::instance()->getDeltaSeconds());
	velocity.y += add;

	//se não está aplicando desaceleração e a aceleração for zero...
	if (add == 0)
	{
		//então aplique a desaceleração
		velocity.y -= velocity.y * damping.y;	
		damped = true;
	}
	
	if (velocity.y > minVel.y && velocity.y > maxVel.y)
		velocity.y = maxVel.y;
	else if (velocity.y < -minVel.y && velocity.y < -maxVel.y)
		velocity.y = -maxVel.y;
	//está abaixo do intervalo
	else if (!damped && add == 0)
	{
		velocity.y -= velocity.y * damping.y;
	}
	
/*
	NOTA IMPORTANTE:
	pra manter o movimento fluido e constante, tem de multiplicar
	a velocidade pelo delta apenas quando vai somar na posição
*/
	position.y += velocity.y * double(FPSManager::instance()->getDeltaSeconds());
}

std::string Entity::getStateString (  )
{
	return "null";
}

std::string Entity::to_json ()
{
	return "";
}

void Entity::input ( SDL_Event & event )
{

}

void Entity::draw ( SDL_Renderer * renderer, Camera * camera )
{
	if (currAnim && isVisible())
		currAnim->draw(renderer, camera, position.x, position.y);
}

int Entity::update (  )
{
	return getState();
}
