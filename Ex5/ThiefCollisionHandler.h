#pragma once
#include "CollisionHandler.h"

class ThiefCollisionHandler : public CollisionHandler
{
private:
	bool m_hasKey;
public:
	ThiefCollisionHandler(EntityPlacer *entityPlacer) : 
		CollisionHandler(entityPlacer), m_hasKey(false) {}

	virtual bool handleCollision(int x, int y)
	{
		bool success = CollisionHandler::handleCollision(x, y);
		if (success)
			return true;
		//picking up the key and openning the gate case
		switch (m_entityPlacer->queryEntity(x, y))
		{
		case Entity::KEY:
			m_hasKey = true;
			return m_entityPlacer->removeEntity(x, y);
		case Entity::GATE:
			success = m_hasKey && m_entityPlacer->removeEntity(x, y);
			m_hasKey = m_hasKey && !success; //if had key, and move failed, still has key
			return success;
		default:
			return false;
		}
	}
};