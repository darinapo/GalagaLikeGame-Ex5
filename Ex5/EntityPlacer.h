#pragma once
#include "Entity.h"

class EntityPlacer
{	
public:
	//Abstract class, will remove the enities from the board
	virtual Entity::Symbol queryEntity(int x, int y) = 0;
	virtual bool removeEntity(int x, int y) = 0;
};
