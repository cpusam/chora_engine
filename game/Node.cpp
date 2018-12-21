#include "Node.hpp"
#include "Elements.hpp"

NodeID Node::countID = 0;

Node::Node (  )
{
	countID++;
	id = countID;
	groups = 0;
	layer = 0;
	name = "Node";
	nextA = nullptr;
	nextB = nullptr;
	parent = nullptr;
}

Node::~Node (  )
{
	Elements::remove(Node::getID());
}

 bool receive ( Node * sender, std::string mesg )
 {
	 return false;
 }

void Node::addGroups ( NodeGroups g )
{
	groups = groups | g;
}

void Node::draw ( SDL_Renderer * renderer, Camera * camera )
{

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

