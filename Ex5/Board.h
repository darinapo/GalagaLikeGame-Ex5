#pragma once
#include "EntityPlacer.h"
#include "CollisionHandlerFactory.h"
#include "Menu.h"

class Board : public EntityPlacer
{
public:
	Board() : m_level(1), m_collisionHandlerFactory(this), m_selectedEntity(-1,-1), m_won(false) {};
	~Board();
	bool init();//constructor
	void print(sf::RenderWindow &window, bool update);//Printing function
	void handleClick(sf::Event &event);//handling all the buttons
	bool handleKey(sf::Event &event);//handaling the pressed key
	virtual Entity::Symbol queryEntity(int x, int y);
	virtual bool removeEntity(int x, int y);
	int getLevel() const { return m_level; }
	int getBoardSize() const;
	void restartLevel() { loadLevel(); }
	void won() { m_won = true; }
	bool didWin() { return m_won; }

private: 

	bool loadLevel();//loading the level
	void moveAutonomousEntities();//moving the dwarfs
	bool repsawnAutonomousEntity(int x, int y);//moving the dwarf back to it's place
	void clearBoard();

	static const unsigned int TILE_SIZE = 32;
	static const char LEVEL_FILENAME[15];//holding the level file name
	
	//The board is build from entities which their type is BLANK 
	
	std::vector<std::vector<Entity>> m_entityBoard;
	//load level "m_level"
	int m_level;
	bool m_won;
	CollisionHandlerFactory m_collisionHandlerFactory;
	sf::Vector2i m_selectedEntity;//the place of the moving character
	std::vector<sf::Vector2i> m_autonomousEntities;
	std::vector<sf::Vector2i> m_autonomousEntitiesSpawns;//holding the data corresponence to the dwarf's array
	sf::Clock m_autonomousEntitiesTimer;
};

