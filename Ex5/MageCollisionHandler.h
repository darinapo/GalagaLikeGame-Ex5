#pragma once
#include "CollisionHandler.h"

class MageCollisionHandler : public CollisionHandler
{
public:

	MageCollisionHandler(EntityPlacer *entityPlacer) :
		CollisionHandler(entityPlacer){}

	//
	virtual bool handleCollision(int x, int y)
	{	//default collision (if it's blank)
		bool success = CollisionHandler::handleCollision(x, y);
		if (success)
			return true;
		//if it was a fire
		return m_entityPlacer->queryEntity(x, y) == Entity::FIRE && m_entityPlacer->removeEntity(x, y);
	}
private:

};