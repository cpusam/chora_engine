#include "Elements.hpp"

Elements * Elements::singleton = nullptr;

Elements::Elements()
{
	//ctor
}

Elements::~Elements()
{
	//dtor
}

Elements * Elements::instance (  )
{
	if (singleton == nullptr)
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

std::map<int, Entity *> & Elements::getEntities (  )
{
	return entities;
}

bool Elements::hasEntity ( Entity * e )
{
	if (e)
		for (auto & it: entities)
			if (e == it.second)
				return true;
	
	return false;
}

void Elements::addEntity ( Entity * e )
{
	if (e)
		entities.insert(std::pair<int,Entity*>(e->getId(), e));
}

Entity * Elements::getEntity ( int id )
{
	return entities[id];
}

Entity * Elements::getEntityByName ( std::string name )
{
	for (auto & it: entities)
		if (it.second->getName() == name)
			return it.second;
	
	return nullptr;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( std::string group )
{
	std::vector<Entity*> entity;
	for (auto & it: entities)
		if (it.second->getGroup() == group)
			entity.push_back(it.second);
	
	return entity;
}


void Elements::remEntity ( int id )
{
	entities.erase(id);
}

void Elements::add ( Entity * e )
{
	if (e == nullptr)
		return;
	std::cout<<"Adicionando "<<e->getId()<<" name = "<<e->getName()<<std::endl;
	instance()->addEntity(e);
}

Entity * Elements::get ( int id )
{
	return instance()->getEntity(id);
}

Entity * Elements::getByName ( std::string name )
{
	return instance()->getEntityByName(name);
}

std::map<int, Entity *> & Elements::getAllEntities (  )
{
	return instance()->getEntities();
}

std::vector<Entity *> Elements::getAllByGroup ( std::string group )
{
	return instance()->getAllEntityByGroup(group);
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

bool Elements::has ( Entity * e )
{
	return instance()->hasEntity(e);
}

void Elements::remove ( int id )
{
	instance()->remEntity(id);
}

void Elements::clear (  )
{
	instance()->clearAll();
}

void Elements::clearAll (  )
{
	entities.clear();
}

void Elements::notifyGroup ( Entity * sender, std::string mesg, std::string group )
{
	std::vector<Entity *> entities = instance()->getAllByGroup(group);
	for (auto *entity: entities)
		if (entity != sender)
			entity->receive(sender,mesg);
}

////////////////////////////////////////////////////////////////////////
void Elements::input ( SDL_Event & event )
{
	std::map<int,Entity*>::iterator it;
	for (it = instance()->getEntities().begin(); it != instance()->getEntities().end(); it++)
		it->second->input(event);
}
void Elements::draw ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<int, std::vector<Entity*> > layers;
	std::map<int,std::vector<Entity *> >::iterator layerIt;
	std::map<int,Entity*>::iterator it;
	for (it = instance()->getEntities().begin(); it != instance()->getEntities().end(); it++)
	{
		layers[it->second->getLayer()].push_back(it->second);
	}
	
	for (layerIt = layers.begin(); layerIt != layers.end(); layerIt++)
	{
		for (std::vector<Entity*>::iterator entity = layerIt->second.begin(); entity != layerIt->second.end(); entity++)
		{
			if (*entity)
				(*entity)->draw(renderer, camera);
		}
	}
}

void Elements::update (  )
{
	std::map<int,Entity*>::iterator it;
	for (it = instance()->getEntities().begin(); it != instance()->getEntities().end(); it++)
		if (it->second)
			it->second->update();
}

void Elements::print (  )
{
	std::map<int,Entity*>::iterator it = instance()->getEntities().begin(), end = instance()->getEntities().end();
	for (; it != end; it++)
		if (it->second)
			std::cout<<"id = "<<it->first<<"|"<<it->second->getName()<<std::endl;
	std::cout<< "Temos "<<instance()->getEntities().size()<<" entidades"<<std::endl;
}
