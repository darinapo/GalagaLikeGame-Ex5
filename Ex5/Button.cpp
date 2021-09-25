#include "Button.h"

//=======================================================================
//Constractor, building a button
//=======================================================================
Button::Button(sf::Texture &texture, sf::Vector2f &vector, 
	sf::IntRect &intRect, MenuAction menuAction)
	: m_buttonPressed(false), m_action(menuAction)
{
	m_buttonSprite.setTexture(texture);
	m_buttonSprite.setTextureRect(sf::IntRect(intRect));
	m_buttonSprite.setPosition(sf::Vector2f(vector));
}
//=======================================================================
//Drawing the button
//=======================================================================
void Button::print(sf::RenderWindow & window)
{
	window.draw(m_buttonSprite);
}
//=======================================================================
//Checking if the x,y are contained in the rectangle of the button
//=======================================================================
bool Button::contains(int x, int y) const 
{ 
	return m_buttonSprite.getGlobalBounds().contains(x, y); 
}
//=======================================================================
//Setting up the color of the pressed button
//=======================================================================
void Button::setPressed(bool state) 
{ 
	m_buttonPressed = state; 
	m_buttonSprite.setColor(sf::Color::Red);
	if (state)
		m_buttonSprite.setColor(sf::Color::Blue);
};