#pragma once
#include <SFML\Graphics.hpp>
#include "Button.h"

class Buttons 
{
public:
	Buttons();
	void print(sf::RenderWindow &window);
	unsigned int getSize() { return m_buttonArray.size(); };
	Button getButton(unsigned int index) { return m_buttonArray[index]; };
	MenuAction whichIsPressed(sf::Event &e);
	
private:
	sf::Texture m_menuTexture;
	std::vector<Button> m_buttonArray;
	Button *m_lastPressed;// saving the last pressed button


};

