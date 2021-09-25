#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class CollisionHandler;

class Entity
{
public:
	//Define to each obkect in the game it's type
	enum Symbol
	{
		BLANK = ' ',
		DWARF = '^',
		FIRE = '*',
		GATE = '#',
		GIANT = 'G',
		KEY = 'F',
		KING = 'K',
		MAGE = 'M',
		THIEF = 'T',
		THRONE = '@',
		WALL = '=',
		INVALID = -1 //Everything that is not a button
	};
	static bool initClass(unsigned int tileSize);//Insert into the hash table

	Entity(Symbol s, int posX, int posY, CollisionHandler *handler);//constructor
	bool move(int xDelta, int yDelta);//getting the postion of the sprite
	Symbol getType() const;//getting the type of the entity
	void deleteHandler();
	void getPosition(int& xPos, int& yPos) const;
	void draw(sf::RenderWindow & window, bool isHighlight = false);
	void setPos(int x, int y) { m_pos = m_prevPos = sf::Vector2f((float)x, (float)y); }
	bool isControllable() const { return m_isMovable; }

private:
	static const sf::Color HIGHLIGHT_COLOR;

	static unsigned int m_tileSize;
	static const char m_types[]; //Saving the types of the objects
	static const char* const m_filePaths[];//Saving the path of the picture
	static const bool m_isMovableArray[];
	static std::map<char, sf::Sprite> m_spriteMap;
	static sf::Texture m_textures[];
	
	//Using a hash table which holds the texture and the type of each objext
	Symbol m_type;
	CollisionHandler *m_handler;
	sf::Vector2f m_pos;
	sf::Vector2f m_prevPos;
	sf::Clock m_timer;
	bool m_isMovable;
};
