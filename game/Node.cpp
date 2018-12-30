#include "Node.hpp"
//#include "Elements.hpp"

NodeID Node::countID = 0;

Node::Node ( std::string name, int initState, NodeGroups groups, NodeType type )
{
	countID++;
	id = countID;
	setState(initState);
	this->groups = groups;
	layer = 0;
	this->name = name;
	this->type = type;
	nextA = nullptr;
	nextB = nullptr;
	parent = nullptr;
}

Node::~Node (  )
{
	#ifdef ELEMENTS_HPP
		Elements::remove(Node::getID());
	#endif
}

bool Node::receive ( Node * sender, std::string mesg )
{
	return false;
}

Node * Node::clone (  )
{
	//retorna um novo Node
	Node * newNode = new Node(name, groups, type);
	#ifdef ELEMENTS_HPP
		Elements::add(newNode);
	#endif
	return newNode;
}

void Node::addGroups ( NodeGroups g )
{
	groups = groups | g;
}

void Node::draw ( SDL_Renderer * renderer, Camera * camera )
{
	
}

//tem de colocar essas funções quando forem usadas engines diferentes
Vect Node::getPosition (  )
{
	return position;
}

float Node::getPositionX (  )
{
	return position.x;
}

float Node::getPositionY (  )
{
	return position.y;
}

void Node::setPosition ( Vect p )
{
	position.set(p.x,p.y);
}

void Node::setPositionX ( float px )
{
	position.x = px;
}

void Node::setPositionY ( float py )
{
	position.y = py;
}

//escala
Vect Node::getScale (  )
{
	return scale;
}

float Node::getScaleX (  )
{
	return scale.x;
}

float Node::getScaleY (  )
{
	return scale.y;
}

bool Node::isVisible (  )
{
	return visible;
}

//verifica se os bits de g estão em groups
bool Node::isInsideGroup ( NodeGroups g )
{
	return groups & g;
}

NodeGroups Node::getGroups (  )
{
	return groups;
}

NodeID Node::getID (  )
{
	return id;
}

std::string Node::getName (  )
{
	return name;
}

Node * Node::getNextA (  )
{
	return nextA;
}

Node * Node::getNextB (  )
{
	return nextB;
}

Node * Node::getParent (  )
{
	return parent;
}

NodeType Node::getType (  )
{
	return type;
}

void Node::setGroup ( bool g, int pos )
{
	groups = groups | (g << pos);
}

int Node::getLayer (  )
{
	return layer;
}

void Node::show ( bool s )
{
	visible = s;
}

void Node::setLayer ( int l )
{
	layer = l;
}

void Node::setName ( std::string name )
{
	this->name = name;
}

void Node::setNextA ( Node * n )
{
	nextA = n;
}

void Node::setNextB ( Node * n )
{
	nextB = n;
}

void Node::setParent ( Node * p )
{
	parent = p;
}

void Node::setType ( NodeType type )
{
	this->type = type;
}

