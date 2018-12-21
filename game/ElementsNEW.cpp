#include "ElementsNEW.hpp"
#include <algorithm>

Elements * Elements::singleton = nullptr;

Elements::Elements()
{
	currCamera = nullptr;
	currRenderer = nullptr;
}

Elements::~Elements()
{
	currCamera = nullptr;
	currRenderer = nullptr;
	printf("Elements::WARNING currCamera e currRenderer agora são nulos\n");
}

Elements * Elements::instance (  )
{
	if (singleton)
		return singleton;
	
	singleton = new Elements();
	return singleton;
}

void Elements::setCurrRenderer ( SDL_Renderer * renderer )
{
	currRenderer = renderer;
}

SDL_Renderer * Elements::getCurrRenderer (  )
{
	return currRenderer;
}

Camera * Elements::getCurrCamera (  )
{
	return currCamera;
}

void Elements::setCurrCamera ( Camera * cam )
{
	if (cam)
		currCamera = cam;
}


std::vector<Node *> Elements::getNodes (  )
{
	return nodes;
}

bool Elements::hasNode ( Node * e )
{
	if (e)
		for (size_t i = 0; i < nodes.size(); i++)
			if (e == nodes[i])
				return true;
	
	return false;
}

void Elements::addNode ( Node * e )
{
	if (e == nullptr)
		return;
	
	if (hasNode(e))
		return;
	
	std::cout<<"Elements::Adicionando "<<e->getID()<<" name = "<<e->getName()<<std::endl;
	nodesID.push_back(e->getID());
	nodes.push_back(e);
	layers[e->getLayer()].push_back(e);
}

Node * Elements::getNode ( NodeID id )
{
	for (auto * entity: nodes)
		if (entity->getID() == id)
			return entity;

	return nullptr;
}

Node * Elements::getNodeByName ( const std::string & name )
{
	for (auto * entity: nodes)
		if (entity->getName() == name)
			return entity;
	
	return nullptr;
}

std::vector<Node *> Elements::getAllNodeByGroup ( const NodeGroups group )
{
	std::vector<Node*> ret;
	for (auto * entity: nodes)
		if (entity->isInsideGroup(group))
			ret.push_back(entity);
	
	return ret;
}


Node * Elements::remNode ( NodeID id )
{
	std::vector<NodeID>::iterator itID = std::find(nodesID.begin(), nodesID.end(), id);
	if (itID == nodesID.end())
		return nullptr;
	
	Node * ret = getNode(id);
	std::vector<Node *>::iterator itEntity = std::find(nodes.begin(), nodes.end(), ret);
	
	std::cout<<"Elements::Removendo "<<ret->getID()<<" name = "<<ret->getName()<<std::endl;
	nodes.erase(itEntity);
	nodesID.erase(itID);
	
	itEntity = std::find(layers[ret->getLayer()].begin(), layers[ret->getLayer()].end(), ret);
	if (itEntity != layers[ret->getLayer()].end())
		layers[ret->getLayer()].erase(itEntity);

	return ret;
}

void Elements::add ( Node * e )
{
	instance()->addNode(e);
}

Node * Elements::get ( NodeID id )
{
	return instance()->getNode(id);
}

Node * Elements::getByName ( const std::string & name )
{
	return instance()->getNodeByName(name);
}

std::vector<Node *> Elements::getAllNodes (  )
{
	return instance()->getNodes();
}

std::vector<Node *> Elements::getAllByGroup ( const NodeGroups groups )
{
	return instance()->getAllNodeByGroup(groups);
}

void Elements::setRenderer ( SDL_Renderer * renderer )
{
	instance()->setCurrRenderer(renderer);
}

SDL_Renderer * Elements::getRenderer (  )
{
	SDL_Renderer * renderer = instance()->getCurrRenderer();

	if (renderer == nullptr)
		printf("Elements::Erro renderer não definido\n");
	
	return renderer;
}

Camera * Elements::getCamera (  )
{
	return instance()->getCurrCamera();
}

void Elements::setCamera ( Camera * cam )
{
	instance()->setCurrCamera(cam);
}

bool Elements::has ( Node * e )
{
	return instance()->hasNode(e);
}

Node * Elements::remove ( NodeID id )
{
	return instance()->remNode(id);
}

void Elements::clear (  )
{
	instance()->clearAll();
}

void Elements::destroy (  )
{
	if (singleton)
	{
		std::vector<Node *> nodes = instance()->getAllNodes();
		for (Node *it: nodes)
			if (it)
			{
				instance()->remNode(it->getID());
				delete it;
			}
		instance()->clearAll();

		delete singleton;
		singleton = nullptr;
	}
}

void Elements::clearAll (  )
{
	nodes.clear();
	nodesID.clear();
}

const std::map<int, std::vector<Node *> > & Elements::getNodeLayers (  )
{
	return layers;
}

std::vector<NodeID> Elements::getIDs (  )
{
	return nodesID;
}

void Elements::notifyGroup ( Node * sender, const std::string & mesg, const NodeGroups groups )
{
	if (groups == ALL)
	{
		std::vector<Node*> all = instance()->getAllNodes();
		for (auto * it: all)
			it->receive(sender, mesg);
	}
	else
	{
		std::vector<Node *> nodes = instance()->getAllByGroup(groups);
	
		for (auto *entity: nodes)
			if (entity != sender)
				entity->receive(sender, mesg);
	}
}

////////////////////////////////////////////////////////////////////////
void Elements::input ( SDL_Event & event )
{
	instance()->inputNodes(event);
}

void Elements::draw ( SDL_Renderer * renderer, Camera * camera )
{
	instance()->drawNodes(renderer, camera);
}

void Elements::update (  )
{
	instance()->updateNodes();
}

void Elements::inputNodes ( SDL_Event & event )
{
	for (size_t i = 0, size = nodes.size(); i < size; ++i)
		if (nodes[i])
			nodes[i]->input(event);
}

void Elements::drawNodes ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<int, std::vector<Node*> > layers = instance()->getNodeLayers();
	
	for (auto & it: layers)
		for (auto * entity: it.second)
			if (entity && renderer && camera && entity->isVisible())
				entity->draw(renderer, camera);
}

void Elements::updateNodes (  )
{
	if (nodes.size())
		for (size_t i = 0, size = nodes.size(); i < size; i++)
			if (nodes[i])
			{
				#ifdef DEBUG_ELEMENTS
				printf("Atualizando %s\n", nodes[i]->getName().c_str());
				#endif
				nodes[i]->update();
				#ifdef DEBUG_ELEMENTS
				printf("Atualizado %s\n\n", nodes[i]->getName().c_str());
				#endif
			}
}

void Elements::print (  )
{
	for (size_t i = 0; i < instance()->getIDs().size(); i++)
		printf("ID[%zd] = %ld\n", i, (instance()->getIDs()[i]));
	std::vector<Node*> nodes = instance()->getNodes();
	std::cout<<"Temos "<<nodes.size()<<" ENTIDADES\n";
	for (size_t i = 0; i < instance()->getNodes().size(); i++)
		if (nodes[i])
		{
			std::cout<<"id = "<<nodes[i]->getID()<<"|"
			         <<nodes[i]->getName()<<"|ptr="
							 <<nodes[i]<<std::endl;
		}
	std::cout<< "Temos "<<nodes.size()<<" entidades"<<std::endl;
}
