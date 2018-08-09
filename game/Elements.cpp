#include "Elements.hpp"

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

std::map<EntityID, Entity *> Elements::getEntities (  )
{
	std::map<EntityID, Entity *> all;
	for (auto & entity: entities)
		if (entity.second)
			all[entity.first] = entity.second;
	return all;
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
	if (e == nullptr)
		return;
	
	std::cout<<"Elements::Adicionando "<<e->getId()<<" name = "<<e->getName()<<std::endl;
	entities[e->getId()] = e;
}

Entity * Elements::getEntity ( EntityID id )
{
	return entities[id];
}

Entity * Elements::getEntityByName ( std::string name )
{
	for (auto & it: entities)
		if (it.second && it.second->getName() == name)
			return it.second;
	
	return nullptr;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( std::string group )
{
	std::vector<Entity*> entity;
	for (auto & it: entities)
		if (it.second && it.second->getGroup() == group)
			entity.push_back(it.second);
	
	return entity;
}


Entity * Elements::remEntity ( EntityID id )
{
	std::map<EntityID,Entity *>::iterator it = entities.find(id);
	
	if (it == entities.end())
		return nullptr;
	
	if (it->second == nullptr)
		return nullptr;

	std::cout<<"Elements::Removendo "<<it->second->getId()<<" name = "<<it->second->getName()<<std::endl;

	Entity * ret = entities[id];
	//entidade anulada até que apague todas as entidades
	//este id fica inutilizável temporariamente
	entities[id] = nullptr;

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

std::map<EntityID, Entity *> Elements::getAllEntities (  )
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
		std::map<EntityID, Entity *> entities = instance()->getAllEntities();
		for (std::pair<EntityID,Entity*> it: entities)
			if (it.second)
				delete it.second;
		instance()->clearAll();

		delete singleton;
		singleton = nullptr;
	}
}

void Elements::clearAll (  )
{
	entities.clear();
}

void Elements::notifyGroup ( Entity * sender, std::string mesg, std::string group )
{
	std::vector<Entity *> entities;
	
	if (group == "ALL")
	{
		std::map<EntityID, Entity*> all = instance()->getAllEntities();
		for (auto it: all)
		{
			entities.push_back(it.second);
		}
	}
	else
		entities = instance()->getAllByGroup(group);
	
	for (auto *entity: entities)
		if (entity != sender)
			entity->receive(sender,mesg);
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
	std::map<EntityID,Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++)
		if (it->second)
			it->second->input(event);
}

void Elements::drawEntities ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<EntityID, std::vector<Entity*> > layers;
	for (std::map<EntityID,Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if (it->second)
			layers[it->second->getLayer()].push_back(it->second);
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
	for (auto & it: entities)
		if (it.second)
			it.second->update();
}

void Elements::print (  )
{
	std::map<EntityID,Entity*>::iterator it = instance()->getEntities().begin(), end = instance()->getEntities().end();
	for (; it != end; it++)
		if (it->second)
			std::cout<<"id = "<<it->first<<"|"<<it->second->getName()<<std::endl;
	std::cout<< "Temos "<<instance()->getEntities().size()<<" entidades"<<std::endl;
}
