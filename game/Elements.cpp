#include "Elements.hpp"
#include <algorithm>

std::atomic<Elements *> Elements::singleton{nullptr};
std::mutex Elements::myMutex;

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
	{
		std::lock_guard<std::mutex> lock(myMutex);
		if (singleton == nullptr)
			singleton = new Elements();
	}
	
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

std::vector<Entity *> Elements::getEntities (  )
{
	return entities;
}

bool Elements::hasEntity ( Entity * e )
{
	if (e)
		for (auto * it: entities)
			if (e == it)
				return true;
	
	return false;
}

void Elements::addEntity ( Entity * e )
{
	if (e == nullptr)
		return;
	
	std::cout<<"Elements::Adicionando "<<e->getId()<<" name = "<<e->getName()<<std::endl;
	entitiesID.push_back(e->getId());
	entities.push_back(e);
}

Entity * Elements::getEntity ( EntityID id )
{
	for (auto * entity: entities)
		if (entity->getId() == id)
			return entity;

	return nullptr;
}

Entity * Elements::getEntityByName ( std::string name )
{
	for (auto * entity: entities)
		if (entity->getName() == name)
			return entity;
	
	return nullptr;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( std::string group )
{
	std::vector<Entity*> ret;
	for (auto * entity: entities)
		if (entity->getGroup() == group)
			ret.push_back(entity);
	
	return ret;
}


Entity * Elements::remEntity ( EntityID id )
{
	std::vector<EntityID>::iterator itID = std::find(entitiesID.begin(), entitiesID.end(), id);
	if (itID == entitiesID.end())
		return nullptr;
	
	Entity * ret = getEntity(id);
	std::vector<Entity *>::iterator itEntity = std::find(entities.begin(), entities.end(), ret);
	
	std::cout<<"Elements::Removendo "<<ret->getId()<<" name = "<<ret->getName()<<std::endl;
	entities.erase(itEntity);
	entitiesID.erase(itID);

	return ret;
}

void Elements::add ( Entity * e )
{
	instance()->addEntity(e);
}

Entity * Elements::get ( EntityID id )
{
	return instance()->getEntity(id);
}

Entity * Elements::getByName ( std::string name )
{
	return instance()->getEntityByName(name);
}

std::vector<Entity *> Elements::getAllEntities (  )
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

Entity * Elements::remove ( EntityID id )
{
	return instance()->remEntity(id);
}

void Elements::clear (  )
{
	instance()->clearAll();
}

void Elements::destroy (  )
{
	if (singleton)
	{
		std::vector<Entity *> entities = instance()->getAllEntities();
		for (Entity *it: entities)
			if (it)
				delete it;
		instance()->clearAll();

		delete singleton;
		singleton = nullptr;
	}
}

void Elements::clearAll (  )
{
	entities.clear();
	entitiesID.clear();
}

void Elements::notifyGroup ( Entity * sender, std::string mesg, std::string group )
{
	std::vector<Entity *> entities;
	
	if (group == "ALL")
	{
		std::vector<Entity*> all = instance()->getAllEntities();
		for (auto * it: all)
		{
			it->receive(sender, mesg);
		}
	}
	else
	{
		entities = instance()->getAllByGroup(group);
	
		for (auto *entity: entities)
			if (entity != sender)
				entity->receive(sender,mesg);
	}
}

////////////////////////////////////////////////////////////////////////
void Elements::input ( SDL_Event & event )
{
	instance()->inputEntities(event);
}

void Elements::draw ( SDL_Renderer * renderer, Camera * camera )
{
	instance()->drawEntities(renderer, camera);
}

void Elements::update (  )
{
	instance()->updateEntities();
}

void Elements::inputEntities ( SDL_Event & event )
{
	std::vector<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++)
		if (*it)
			(*it)->input(event);
}

void Elements::drawEntities ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<EntityID, std::vector<Entity*> > layers;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if (*it)
			layers[(*it)->getLayer()].push_back(*it);
	}
	
	for (auto & it: layers)
	{
		for (auto * entity: it.second)
		{
			if (entity && renderer && camera)
				entity->draw(renderer, camera);
		}
	}
}

void Elements::updateEntities (  )
{
	for (auto * it: entities)
		if (it)
			it->update();
}

void Elements::print (  )
{
	std::vector<Entity*>::iterator it = instance()->getEntities().begin(), end = instance()->getEntities().end();
	for (; it != end; it++)
		if (*it)
			std::cout<<"id = "<<(*it)->getId()<<"|"<<(*it)->getName()<<std::endl;
	std::cout<< "Temos "<<instance()->getEntities().size()<<" entidades"<<std::endl;
}
