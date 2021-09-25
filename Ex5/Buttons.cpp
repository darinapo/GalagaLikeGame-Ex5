#include "Buttons.h"
//=======================================================================
//Constractor
//=======================================================================
Buttons::Buttons()
{
	if (!m_menuTexture.loadFromFile("menu.png"))
	{
		exit(1);
	}
	// pushing the buttons into an array
	m_buttonArray.push_back(Button{m_menuTexture,sf::Vector2f(500,300), sf::IntRect(10,5,420,100), MENU_START });
	m_buttonArray.push_back(Button{ m_menuTexture,sf::Vector2f(500,500), sf::IntRect(10,150,400,70), MENU_ABOUT });
	m_buttonArray.push_back(Button{ m_menuTexture,sf::Vector2f(500,700), sf::IntRect(20,250,400,60), MENU_EXIT });
}
//=======================================================================
//Printing the array of buttons
//=======================================================================
void Buttons::print(sf::RenderWindow & window)
{
	for (unsigned int i = 0; i < m_buttonArray.size(); ++i)
	{
		m_buttonArray[i].print(window);
	}

}
//=======================================================================
//Checking which button is pressed
//=======================================================================
MenuAction Buttons::whichIsPressed(sf::Event & e)
{
	//Going threw the array and checks if the mouse click event was on one of the buttons
	for (unsigned int i = 0; i < m_buttonArray.size(); i++) {
		if (m_buttonArray[i].contains(e.mouseButton.x, e.mouseButton.y))
		{
			if(NULL != m_lastPressed)
				m_lastPressed->setPressed(false);
			m_lastPressed = &m_buttonArray[i];
			m_lastPressed->setPressed(true);
			return m_lastPressed->getAction();//returning the "new" last pressed button
		}
	}
	//If didn't press on any of the buttons
	return MenuAction::MENU_INVALID;
}