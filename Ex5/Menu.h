#pragma once
#include <SFML\Graphics.hpp>
#include "Buttons.h"


class Menu 
{
public:

	Menu();
	void drawMenu(sf::RenderWindow & window);
	Buttons getButtons();


private:
	Buttons m_buttons;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	int m_lastPressed;

};

