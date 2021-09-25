#pragma once
#include "CollisionHandler.h"

class KingCollisionHandler : public CollisionHandler
{
public:

	KingCollisionHandler(EntityPlacer *entityPlacer) :
		CollisionHandler(entityPlacer), m_onThrone(false) {}


	virtual bool handleCollision(int x, int y)
	{
		//default collision (if it's blank)
		bool success = CollisionHandler::handleCollision(x, y);
		if (success)
			return true;
		//if it's not a throne or blank, the king cannot walk to it
		if (m_entityPlacer->queryEntity(x, y) != Entity::THRONE)
			return false;
		return m_entityPlacer->removeEntity(x, y);
	}
private:
	bool m_onThrone;
};