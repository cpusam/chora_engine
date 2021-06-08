#include "Elements.hpp"
#include <algorithm>

Elements * Elements::singleton = nullptr;

Elements::Elements()
{
	currCamera = nullptr;
}

Elements::~Elements()
{
	//dtor
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


std::vector<Entity *> Elements::getEntities (  )
{
	return entities;
}

bool Elements::hasEntity ( Entity * e )
{
	if (e)
		for (size_t i = 0; i < entities.size(); i++)
			if (e == entities[i])
				return true;
	
	return false;
}

void Elements::addEntity ( Entity * e )
{
	if (e == nullptr)
		return;
	
	if (hasEntity(e))
		return;
	
	std::cout<<"Elements::Adicionando "<<e->getID()<<" name = "<<e->getName()<<std::endl;
	entitiesID.push_back(e->getID());
	entities.push_back(e);
	if (std::find(layers[e->getLayer()].begin(), layers[e->getLayer()].end(), e) == layers[e->getLayer()].end())
		layers[e->getLayer()].push_back(e);
}

Entity * Elements::getEntity ( EntityID id )
{
	for (auto * entity: entities)
		if (entity->getID() == id)
			return entity;

	return nullptr;
}

Entity * Elements::getEntityByName ( const std::string & name )
{
	for (auto * entity: entities)
		if (entity->getName() == name)
			return entity;
	
	return nullptr;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( const std::string & group )
{
	std::vector<Entity*> ret;
	for (auto * entity: entities)
		if (entity->getGroup() == group)
			ret.push_back(entity);
	
	return ret;
}

std::vector<Entity *> Elements::getAllEntityByGroup ( const NodeGroups group, bool onlyVisible )
{
	std::vector<Entity*> ret;
	for (auto * entity: entities)
	{
		if (onlyVisible)
		{
			if (entity->isVisible() && entity->insideGroup(group))
				ret.push_back(entity);
		}
		else
		{
			if (entity->insideGroup(group))
				ret.push_back(entity);
		}
	}
			
	
	return ret;
}


Entity * Elements::remEntity ( EntityID id )
{	
	Entity * ret = getEntity(id);
	if (!ret)
		return nullptr;
	
	
	auto it = std::find(entities.begin(), entities.end(), ret);
	if (it != entities.end()) {
		eraseEntity.push_back(ret);
		entities.erase(it);
	}

	return ret;
}

void Elements::updateErase ()
{
	if (1)
		return;
	
	for (std::vector<Entity *>::iterator it = eraseEntity.begin(), end = eraseEntity.end(); it != end; ++it)
	{
		Entity * ret = *it;
		if (!ret)
			continue;
		
		std::vector<Entity *>::iterator itEntity = std::find(entities.begin(), entities.end(), ret);

		if (itEntity != entities.end())
		{
			std::cout<<"Elements::Removendo "<<ret->getID()<<" name = "<<ret->getName()<<std::endl;
			entities.erase(itEntity);
			std::vector<EntityID>::iterator itID = std::find(entitiesID.begin(), entitiesID.end(), ret->getID());
			if (itID != entitiesID.end())
				entitiesID.erase(itID);
		}
		else
			continue;
		
		std::vector<Entity *> & layerEntities = layers[ret->getLayer()];
		itEntity = std::find(layerEntities.begin(), layerEntities.end(), ret);
		if (itEntity != layerEntities.end())
		{
			layerEntities.erase(itEntity);
		}
	}

	if (eraseEntity.size())
		eraseEntity.clear();
}

void Elements::add ( Entity * e )
{
	instance()->addEntity(e);
}

Entity * Elements::get ( EntityID id )
{
	return instance()->getEntity(id);
}

Entity * Elements::getByName ( const std::string & name )
{
	return instance()->getEntityByName(name);
}

std::vector<Entity *> Elements::getAllEntities (  )
{
	return instance()->getEntities();
}

std::vector<Entity *> Elements::getAllByGroup ( const std::string & group )
{
	return instance()->getAllEntityByGroup(group);
}

std::vector<Entity *> Elements::getAllByGroup ( const NodeGroups groups, bool onlyVisible )
{
	return instance()->getAllEntityByGroup(groups, onlyVisible);
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
			{
				instance()->remEntity(it->getID());
				delete it;
			}
		instance()->clearAll();

		delete singleton;
		singleton = nullptr;
	}
}

void Elements::clearAll (  )
{
	entities.clear();
	entitiesID.clear();
	layers.clear();
}

const std::map<int, std::vector<Entity *> > & Elements::getEntitiesLayers (  )
{
	return layers;
}

std::vector<EntityID> Elements::getIDs (  )
{
	return entitiesID;
}

void Elements::notifyGroup ( Entity * sender, const std::string & mesg, const std::string & group )
{
	if (group == "ALL")
	{
		std::vector<Entity*> all = instance()->getAllEntities();
		for (auto * it: all)
			if (it)
				it->receive(sender, mesg);
	}
	else
	{
		std::vector<Entity *> entities = instance()->getAllByGroup(group);
	
		for (auto *entity: entities)
			if (entity && entity != sender)
				entity->receive(sender, mesg);
	}
}


void Elements::notifyGroup ( Entity * sender, const std::string & mesg, const NodeGroups & group )
{
	std::vector<Entity *> entities = instance()->getAllByGroup(group);

	for (auto *entity: entities)
		if (entity != sender)
			entity->receive(sender, mesg);
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
	for (size_t i = 0, size = entities.size(); i < size; ++i)
		if (entities[i])
			entities[i]->input(event);
}

void Elements::drawEntities ( SDL_Renderer * renderer, Camera * camera )
{
	std::map<int, std::vector<Entity*> > layers = instance()->getEntitiesLayers();
	#if DEBUG
	std::map<Entity *, int> calls;
	#endif
	if (renderer && camera)
		for (auto & it: layers)
			for (auto * entity: it.second)
				if (entity != nullptr)
				{
					#if DEBUG
					calls[entity]++;
					#endif
					if (entity->isVisible())
						entity->draw(renderer, camera);
				}
	
	#ifdef DEBUG
	std::cout<<"Elements::drawEntities\n";
	for (auto & it: calls)
		if (it.second && it.first)
			std::cout<<"Para "<<it.first->getName()<<" foram "<<it.second<<" chamadas\n";
	#endif
}

//#define DEBUG_ELEMENTS
void Elements::updateEntities (  )
{
	for (size_t i = 0, size = entities.size(); i < size; i++)
		if (entities[i])
		{
			if (std::find(eraseEntity.begin(), eraseEntity.end(), entities[i]) != eraseEntity.end())
				continue;
			#ifdef DEBUG_ELEMENTS
			//std::cout<<"[PTR] "<<(*entities[i])<<std::endl;
			std::cout<<"Atualizando "<<entities[i]->getName()<<" id="<<entities[i]->getID()<<std::endl;
			#endif

			entities[i]->updatePosition();
			entities[i]->update();

			#ifdef DEBUG_ELEMENTS
			printf("Atualizado %s\n\n", entities[i]->getName().c_str());
			#endif
		}
	
	//remove de forma segura as entidades excluidas
	updateErase();
}

void Elements::print (  )
{
	for (size_t i = 0; i < instance()->getIDs().size(); i++)
		printf("ID[%zd] = %ld\n", i, (instance()->getIDs()[i]));
	std::vector<Entity*> entities = instance()->getEntities();
	std::cout<<"Temos "<<entities.size()<<" ENTIDADES\n";
	for (size_t i = 0; i < instance()->getEntities().size(); i++)
		if (entities[i])
		{
			std::cout<<"id = "<<entities[i]->getID()<<"|"
			         <<entities[i]->getName()<<"|ptr="
							 <<entities[i]<<std::endl;
		}
	std::cout<< "Temos "<<entities.size()<<" entidades"<<std::endl;
}
