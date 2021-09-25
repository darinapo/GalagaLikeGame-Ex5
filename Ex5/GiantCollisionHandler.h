#pragma once
#include "CollisionHandler.h"

class GiantCollisionHandler : public CollisionHandler
{
public:

	GiantCollisionHandler(EntityPlacer *entityPlacer) :
		CollisionHandler(entityPlacer) {}


	virtual bool handleCollision(int x, int y)
	{
		//default collision (if it's blank)
		bool success = CollisionHandler::handleCollision(x, y);
		if (success)
			return true;
		//the giant can get on a dwarf 
		return m_entityPlacer->queryEntity(x, y) == Entity::DWARF &&
			m_entityPlacer->removeEntity(x, y);//removing the dwarfs
	}
};
