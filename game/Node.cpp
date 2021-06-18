#include "Node.hpp"
#include "Elements.hpp"
#include <algorithm>

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
	parent = nullptr;
	show(true);
}

Node::~Node (  )
{
	Elements::remove(getID());
}

bool Node::receive ( Node * sender, std::string mesg )
{
	return false;
}

Node * Node::clone (  )
{
	//retorna um novo Node
	Node * newNode = new Node(name, groups, type);
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
const Vect & Node::getScale (  )
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
bool Node::insideGroup ( NodeGroups g )
{
	return (groups & g) != 0;
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

std::vector<Node *> const & Node::getChildren (  )
{
	return children;
}

void Node::addChild ( Node * child )
{
	if (child && std::find(children.begin(), children.end(), child) == children.end())
		children.push_back(child);
}

void Node::removeChild ( Node * child )
{
	std::vector<Node *>::iterator it = std::find(children.begin(), children.end(), child);

	if (it != children.end())
		children.erase(it);
}

void Node::updateChildrenPosition (  )
{
	//TODO multiplicar a matriz deste node pela posição dos child
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

void Node::setParent ( Node * p )
{
	parent = p;
}

void Node::setType ( NodeType type )
{
	this->type = type;
}

