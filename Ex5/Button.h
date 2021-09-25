#pragma once
#include <SFML\Graphics.hpp>
#include "MenuActions.h"

class Button 
{
public:
	Button(sf::Texture &texture, sf::Vector2f &, sf::IntRect &, MenuAction);
	void print(sf::RenderWindow &window);
	bool contains(int x, int y) const;//checks if the click is in the button limits
	void setPressed(bool state);
	MenuAction getAction() const { return m_action; }
private:
	sf::Sprite m_buttonSprite;
	bool m_buttonPressed;
	MenuAction m_action;
};
