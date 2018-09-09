#include "Elements.hpp"
#include <algorithm>

#if defined(WIN32) || defined(WIN64)
	Elements * Elements::singleton;
#else
	std::atomic<Elements *> Elements::singleton{nullptr};
	std::mutex Elements::myMutex;
#endif

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
	#if defined(WIN32) || defined(WIN64) || defined(NO_THREAD_SAFE)
		if (singleton == nullptr)
			singleton = new Elements();
		
		return singleton;
	#else
    Elements * tmp = singleton.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (tmp == nullptr)
		{
        std::lock_guard<std::mutex> lock(myMutex);
        tmp = singleton.load(std::memory_order_relaxed);
        if (tmp == nullptr)
				{
            tmp = new Elements();
            std::atomic_thread_fence(std::memory_order_release);
            singleton.store(tmp, std::memory_order_relaxed);
        }
    }
		
    return tmp;
	#endif
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


std::vector<Entity *> Elements::getEntities (  )
{
	return entities;
}

bool Elements::hasEntity ( Entity * e )
{
	if (e)
		for (int i = 0; i < entities.size(); i++)
			if (e == entities[i])
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

Camera * Elements::getCamera (  )
{
	return instance()->getCurrCamera();
}

void Elements::setCamera ( Camera * cam )
{
	instance()->setCurrCamera(cam);
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

std::vector<EntityID> Elements::getIDs (  )
{
	return entitiesID;
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
	for (int i = 0; i < entities.size(); i++)
		if (entities[i])
			entities[i]->input(event);
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
	if (entities.size())
		for (int i = 0; i < entities.size(); i++)
			if (entities[i])
				entities[i]->update();
}

void Elements::print (  )
{
	for (int i = 0; i < instance()->getIDs().size(); i++)
		printf("ID[%d] = %ld\n", i, (instance()->getIDs()[i]));
	std::vector<Entity*> entities = instance()->getEntities();
	std::cout<<"Temos "<<entities.size()<<" ENTIDADES\n";
	for (int i = 0; i < instance()->getEntities().size(); i++)
		if (entities[i])
		{
			std::cout<<"id = "<<entities[i]->getId()<<"|"
			         <<entities[i]->getName()<<"|ptr="
							 <<entities[i]<<std::endl;
		}
	std::cout<< "Temos "<<entities.size()<<" entidades"<<std::endl;
}
