#pragma once

#include "ThiefCollisionHandler.h"
#include "KingCollisionHandler.h"
#include "MageCollisionHandler.h"
#include "GiantCollisionHandler.h"

class CollisionHandlerFactory
{
public:
	CollisionHandlerFactory(EntityPlacer *entityPlacer) : m_entityPlacer(entityPlacer) {}
	CollisionHandler* buildHandler(Entity::Symbol symbol)
	{
		//Each symbol will send the needed collision by the symbol
		switch (symbol)
		{
		case Entity::THIEF:
			return new ThiefCollisionHandler(m_entityPlacer);
		case Entity::KING:
			return new KingCollisionHandler(m_entityPlacer);
		case Entity::MAGE:
			return new MageCollisionHandler(m_entityPlacer);
		case Entity::GIANT:
			return new GiantCollisionHandler(m_entityPlacer);
		case Entity::DWARF:
			return new CollisionHandler(m_entityPlacer);
		case Entity::BLANK:
		case Entity::WALL:
		default:
			return NULL;
		}
	}

private:
	EntityPlacer *m_entityPlacer;
};