#pragma once

#include "EntityPlacer.h"

class CollisionHandler
{
public:
	CollisionHandler(EntityPlacer *entityPlacer) : m_entityPlacer(entityPlacer) {}
	virtual bool handleCollision(int x, int y) 
	{
		//default collision - everyone can walk on a blank
		return (Entity::BLANK == m_entityPlacer->queryEntity(x, y) &&
			m_entityPlacer->removeEntity(x, y));
	}
protected:
	EntityPlacer *m_entityPlacer;
};

