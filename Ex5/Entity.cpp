#include "Entity.h"
#include "CollisionHandler.h"
#include "EntityTimer.h"

//=====================================================
//Saving the characters types
const char Entity::m_types[] =	 { BLANK,DWARF,FIRE, GATE, GIANT,KEY, KING,MAGE,THIEF,THRONE,WALL };
const bool 
	Entity::m_isMovableArray[] = { false,false,false,false,true,false,true,true,true, false, false};

const sf::Color Entity::HIGHLIGHT_COLOR = sf::Color::Red;
//=====================================================
/*
Saving the path to the picture of each object in match of the savings of the types
(Easy to see)
*/
//=====================================================
const char* const Entity::m_filePaths[] = 	{ "blank.png", "trooper.png", "fire.png", 
											  "gate.png", "giant.png", "key.png", 
											  "lie.png", "yoda.png", "thief.png", 
											  "thron.png", "wall.png" };
//=====================================================
//Prototype of the hash table
//=====================================================

std::map<char, sf::Sprite> Entity::m_spriteMap;
sf::Texture Entity::m_textures[sizeof(m_types)/sizeof(*m_types)];

//=====================================================

//=====================================================
unsigned int Entity::m_tileSize = 0;
//=====================================================
//Inserting into the hash table the sprites we need, it will return false if there was a problem 
//=====================================================
bool Entity::initClass(unsigned int tileSize)
{
	m_tileSize = tileSize;
	/*
	Inserting into the hash table each type which will use as the key to find each texture,
	while loadin it , if there was any problem to load a texture we will return false
	*/
	for (int i = 0; i < sizeof(m_types) / sizeof(*m_types); i++)
	{
		m_spriteMap.insert(std::pair<char, sf::Sprite>(m_types[i], sf::Sprite()));
		m_textures[i] = sf::Texture();
		sf::Image im;
		if (!im.loadFromFile(m_filePaths[i]))
		{
			return false;
		}
		//setting up the backgroung of the sprite as transperant and cutting the needed texture
		im.createMaskFromColor(im.getPixel(0, 0));
		m_textures[i].loadFromImage(im);
		sf::Sprite& sprite = m_spriteMap.at(m_types[i]);
		sprite.setTexture(m_textures[i]);
		sprite.setTextureRect(sf::IntRect( 0, 0 , 32, 32));
		if(m_types[i] == BLANK)
			sprite.setColor(sf::Color(0, 0, 0, 0));
	}
	return true;
}
//=====================================================
//Building the entity
//=====================================================
Entity::Entity(Symbol s, int posX, int posY, CollisionHandler *handler) :
	m_type(s),
	m_handler(handler),
	m_pos(posX, posY),
	m_prevPos(posX, posY)
{ 
	//Checking if we can move the entity
	for (int i = 0; i < sizeof(m_types) / sizeof(*m_types); i++)
	{
		if (m_types[i] == s)
		{
			m_isMovable = m_isMovableArray[i];
			break;
		}
	}
}
//=======================================================================
//Function for setting the position of the entity's sprite
//=======================================================================
bool Entity::move(int xDelta, int yDelta)
{
	//if it could make the move 
	if (NULL == m_handler || !m_handler->handleCollision(m_pos.x + xDelta, m_pos.y + yDelta))
		return false;
	m_timer.restart();
	m_prevPos = m_pos;
	m_pos.x += xDelta;
	m_pos.y += yDelta;
	return true;
}
//=======================================================================
//Drawing the entity in move (the sprite)
//=======================================================================
void Entity::draw(sf::RenderWindow & window, bool isHighlight)
{
	//Will represent the full move
	float transitionFactor = 1;
	////
	transitionFactor = (float)m_timer.getElapsedTime().asMicroseconds() / ENTITY_TICK.asMicroseconds();
	if (transitionFactor >= 1)//if the move is done
		m_prevPos = m_pos;

	transitionFactor = fmin(transitionFactor, 1);
	sf::Sprite& sprite = m_spriteMap.at(m_type);//getting the needed sprite from the hash table
	sf::Vector2f interpolatedPos = 
		transitionFactor*m_pos + (1 - transitionFactor)*m_prevPos;//??????????????????????????/
	//Setting up the position of the sprite 
	sprite.setPosition(interpolatedPos.x * m_tileSize, interpolatedPos.y * m_tileSize);
	
	sf::Color prevColor;
	if (isHighlight)
	{
		prevColor = sprite.getColor();//Getting the previouse color of the sprite
		sprite.setColor(HIGHLIGHT_COLOR);//setting up the sprtite's color as red
	}
	window.draw(sprite);

	//
	if (isHighlight)
		sprite.setColor(prevColor);//setting up the previouse color
}

//=======================================================================
//Function for getting the type of the entity
//=======================================================================
Entity::Symbol Entity::getType() const
{
	return m_type;
}
//=======================================================================
//Deleting the handler pointer
//=======================================================================
void Entity::deleteHandler() 
{ 
	delete m_handler; 
	m_handler = NULL;  
}
//=======================================================================
//Setting up the position
//=======================================================================
void Entity::getPosition(int& xPos, int& yPos) const
{
	xPos = m_pos.x;
	yPos = m_pos.y;
}
