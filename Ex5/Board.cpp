#include "Board.h"
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "EntityTimer.h"

using std::string;

//=====================================================
//The file name that we will save to it the chosen board
const char Board::LEVEL_FILENAME[] = "level%d.stk";
//=====================================================

//=======================================================================
// operator overriding
//=======================================================================

Board::~Board()
{
	clearBoard();
}
//=======================================================================
//Checking if we could make the hash table of sprites, and load the level
//=======================================================================

bool Board::init()
{
	if (!Entity::initClass(TILE_SIZE))
	{
		std::cout << "error initializing drawings" << std::endl;
		return false;
	}
	return loadLevel();
};

//=======================================================================
//Printing the board , and moving the dwarfs
//=======================================================================
void Board::print(sf::RenderWindow & window, bool update)
{
	if(update)
		moveAutonomousEntities();

	//Goes through all the vector and print it, including the marked player
	for (unsigned int y = 0 ; y < m_entityBoard.size() ; y++)
	{
		for (unsigned int x = 0 ; x < m_entityBoard[y].size() ; x++)
			m_entityBoard[y][x].draw(window, m_selectedEntity.x == x && m_selectedEntity.y == y);
	}
}
//=====================================================
/*
The function is getting the event and the entity,it will check if the mouse pressed 
has beeing on the board, if yes it will set up on the board the temp (the needed sprite's
type and sprite)
*/
//=====================================================
void Board::handleClick(sf::Event & event)
{
	int y = event.mouseButton.y / TILE_SIZE;
	int x = event.mouseButton.x / TILE_SIZE;
	if (y < 0 || y >= (int)m_entityBoard.size()
		|| x < 0 || x >= (int)m_entityBoard[y].size() ||
		!m_entityBoard[y][x].isControllable())
		return;
	
	m_selectedEntity.x = x;
	m_selectedEntity.y = y;
}
//=======================================================================
//The function is checking which key was pressed and according to it moving 
//the character
//=======================================================================
bool Board::handleKey(sf::Event &event)
{
	//if no character is moving
	if (-1 == m_selectedEntity.x)
		return false;
	int xDelta = 0, yDelta = 0;
	switch (event.key.code)
	{
	case sf::Keyboard::Down:
		yDelta = 1;
		break;
	case sf::Keyboard::Up:
		yDelta = -1;
		break;
	case sf::Keyboard::Left:
		xDelta = -1 ;
		break;
	case sf::Keyboard::Right:
		xDelta = 1 ;
		break;
	default:
		return false;
	}
	int oldPosX, oldPosY;
	Entity *currEntity = &m_entityBoard[m_selectedEntity.y][m_selectedEntity.x];
	currEntity->getPosition(oldPosX, oldPosY);//getting the previous position
	if (!currEntity->move(xDelta, yDelta))//moving the charcter by it's own rules
		return false;
	//Setting up the moved entity to the new place
	m_entityBoard[oldPosY + yDelta][oldPosX + xDelta] = m_entityBoard[oldPosY][oldPosX];

	//Setting up blank at the previouse place of the moved entity
	m_entityBoard[oldPosY][oldPosX] = Entity(Entity::BLANK, oldPosX, oldPosY, NULL);

	//Changing to a new position of the selcted entity (the moving one)
	m_selectedEntity.x += xDelta;
	m_selectedEntity.y += yDelta;
	return true;
}
//=======================================================================
//Getting the type of the entity in the specific place on the board
//=======================================================================
Entity::Symbol Board::queryEntity(int x, int y)
{
	// if out of the board range
	if (y < 0 || y >= (int)m_entityBoard.size()
		|| x < 0 || x >= (int)m_entityBoard[y].size())
		return Entity::INVALID;
	return m_entityBoard[y][x].getType();
}
//=======================================================================
//Removing the entity from the board
//=======================================================================
bool Board::removeEntity(int x, int y)
{
	// if out of the board range
	if (y < 0 || y >= (int)m_entityBoard.size()
		|| x < 0 || x >= (int)m_entityBoard[y].size())
		return false;
	//
	if (!repsawnAutonomousEntity(x, y))
		return false;
	//the entities we cannot delete
	switch (m_entityBoard[y][x].getType())
	{
	case Entity::WALL:
		return false;
	case Entity::THRONE:
		m_level++;
		if (!loadLevel()) {
			m_level--;
			m_won = true;
		}
		return false;
	}
	//Putting blank at the place that the charcter can move to
	m_entityBoard[y][x] = Entity(Entity::BLANK, x, y, NULL);
	return true;
}
//=======================================================================
//Getting the board cells count
//=======================================================================
int Board::getBoardSize() const
{
	int boardSize = 0;
	for (unsigned int i = 0; i < m_entityBoard.size(); i++)
		boardSize += m_entityBoard[i].size();
	return boardSize;
}

//============ PRIVATE METHODS ==============

//=======================================================================
//Getting the level from the file, it will return flase if there was a problem
//=======================================================================
bool Board::loadLevel()
{
	m_selectedEntity.x = m_selectedEntity.y = -1;
	clearBoard();
	m_autonomousEntities.clear();
	//We can support from 1-999 levels
	if (m_level < 1 || m_level >= 1000)
		return false;

	char filename[sizeof(LEVEL_FILENAME) + 1];
	//loading to the filename the name of the file (the difference is the level number) using c
	sprintf_s(filename, LEVEL_FILENAME, m_level);

	std::ifstream file(filename);//openning the needed file
	if (!file.good())
		return false;

	string line;
	int lineNum = 0;
	int lineCol = 0;
	while (file.good() && !file.eof())
	{
		std::getline(file, line);
		int add = 0;
		if (line == "")
			continue;
		m_entityBoard.push_back(std::vector<Entity>());
		std::vector<Entity> *newVec = &m_entityBoard.back();
		for (unsigned int i = 0; i < line.size(); i++)
		{
			Entity::Symbol s = (Entity::Symbol)line[i];
			newVec->push_back(Entity(s, i, lineNum, m_collisionHandlerFactory.buildHandler(s)));
			if (s == Entity::KING)
			{
				m_selectedEntity.x = i;
				m_selectedEntity.y = lineNum;
			}
			//Pushing into 1st vector the dwarfs, and to the 2nd their 1st position(to make the return there)
			else if (s == Entity::DWARF)
			{
				m_autonomousEntities.push_back(sf::Vector2i(i, lineNum));
				m_autonomousEntitiesSpawns.push_back(sf::Vector2i(i, lineNum));
			}
		}
		lineNum++;//moving to the next line
	}
	return true;
}
//=======================================================================
//Moving the dwarfs 
//=======================================================================
void Board::moveAutonomousEntities()
{
	if (m_autonomousEntitiesTimer.getElapsedTime() > ENTITY_TICK)
	{
		//Putting the ways we can move into an array
		static const sf::Keyboard::Key KEYS[] = { sf::Keyboard::Up, sf::Keyboard::Down,
			sf::Keyboard::Left, sf::Keyboard::Right };
		
		sf::Vector2i selectedEntityBackup = m_selectedEntity;//saving the place it was
		sf::Event event;
		//Going threw all the dwarfs in the array and moving it in a random way
		for (auto it = m_autonomousEntities.begin(); it != m_autonomousEntities.end(); it++)
		{
			//We are getting 4(the possbility ways of moves)
			int randIndex = rand() % (sizeof(KEYS) / sizeof(*KEYS));
			event.key.code = KEYS[randIndex];//Getting the random move
			m_selectedEntity = *it;
			for (int i = 1; i <= sizeof(KEYS) / sizeof(*KEYS) && !handleKey(event); i++)
			{
				event.key.code = KEYS[(randIndex + i) % (sizeof(KEYS) / sizeof(*KEYS))];
			}
			*it = m_selectedEntity;
		}
		m_selectedEntity = selectedEntityBackup;
		m_autonomousEntitiesTimer.restart();
	}
}
//=======================================================================
//The function is putting back the dwraf in it's original place (when the giant is moving them)
//It will put it at it's original place , if it's not vacant , it will look for a new place by rand func
//=======================================================================
bool Board::repsawnAutonomousEntity(int x, int y)
{
	unsigned int i;
	//going through all the dwarf array 
	for (i = 0; i < m_autonomousEntities.size(); i++)
	{
		if (m_autonomousEntities[i] == sf::Vector2i(x, y))
			break;
	}
	//if we got to the end of the dwarfs array, means we didn't need to move them
	if (i == m_autonomousEntities.size())
		return true;
	//if the dwarf cannot get to it's original place, it will look for a vacant place by rand func
	sf::Vector2i respawn = m_autonomousEntitiesSpawns[i];
	if (m_entityBoard[respawn.y][respawn.x].getType() != Entity::BLANK)
	{
		int randY = rand() % m_entityBoard.size();
		int randX = rand() % m_entityBoard[randY].size();
		respawn.x = randX;
		respawn.y = randY;
		//if the place is not avaliable to put the dwars we will go to the next one 
		if (m_entityBoard[respawn.y][respawn.x].getType() != Entity::BLANK)
		{
			do
			{
				respawn.x++;
				if (respawn.x == m_entityBoard[respawn.y].size())
				{
					respawn.x = 0;
					respawn.y = (respawn.y + 1) % m_entityBoard.size();
				}
			} while (m_entityBoard[respawn.y][respawn.x].getType() != Entity::BLANK &&
				(respawn.x != randX || respawn.y != randY));
			if (respawn.x == randX && respawn.y == randY)//couldn't move randomaly the dwarf
			{
				return false;
			}
		}
	}
	//Setting up the position of the dwarf sprite, it's positiong on the board and it's position in it's array
	m_entityBoard[y][x].setPos(respawn.x, respawn.y);
	m_entityBoard[respawn.y][respawn.x] = m_entityBoard[y][x];
	m_autonomousEntities[i] = respawn;
	return true;
}
//=======================================================================
//Deleting the pointers of all the entites at the board
//=======================================================================
void Board::clearBoard()
{
	for (auto line = m_entityBoard.begin(); line != m_entityBoard.end(); line++)
	{
		for (auto entityIt = line->begin(); entityIt != line->end(); entityIt++)
			entityIt->deleteHandler();
	}
	m_entityBoard.clear();
}