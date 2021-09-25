#include "Menu.h"

Menu::Menu()
	:m_lastPressed(0)
	//setting the menu background
	{
		if (!m_texture.loadFromFile("trooperwall.jpg"))
		{
			exit(2);
		}
		
		m_sprite.setTexture(m_texture);
		
	}
//=======================================================================
//Drawing the menu and the buttons
//=======================================================================
void Menu::drawMenu(sf::RenderWindow & window)
{
		window.draw(m_sprite);
		m_buttons.print(window);
}
//=======================================================================
//Returning a button
//=======================================================================
Buttons Menu::getButtons()
{
	return m_buttons;
}