#include "About.h"

About::About()
{ //Setting up the about text
	if (!m_texture.loadFromFile("aboutWallPaper.jpeg"))
	{
		exit(2);
	}
	m_sprite.setTexture(m_texture);

	if (!m_font.loadFromFile("font.otf"))
	{
		exit(1);
	}
	m_text.setString("A long time ago, in a galaxy far, far away...\n It is a period of civil war.\ Rebel spaceships,\n striking from a hidden spaceships,\n striking from a hidden base,n have won their first victory against the evil Galactic Empire. \n During the battle, Rebel spies managed to steal secret plans to the Empire's \nultimate weapon, the Death Star, an armored space station with enough power to destroy an entire planet.\n Pursued by the Empire's sinister agents, Princess Leia races home aboard her \n starship, custodian of the stolen plans that can save her people and restore freedom to the galaxy....\n\n\n Help to Princess Leia to get with the plans to her spaceship\n \n");
	m_text.setFont(m_font);
	m_text.setCharacterSize(30);
	m_text.setFillColor(sf::Color::Yellow);
	m_text.setPosition(0, 1000);
}
//==============================================
void About::print(sf::RenderWindow & window)
{
	window.draw(m_sprite);
	window.draw(m_text);
}
//===================================================
//Running the about 
//===================================================
void About::runAbout()
{
	sf::RenderWindow window(sf::VideoMode(1400, 1000), "About");
	sf::Clock textTimer;
	float speed = 100.0f;
	int textHight = window.getSize().y + 10;
	int secondsLimit = (1.5f*textHight / speed);
	sf::Event event;
	while (window.isOpen() && textTimer.getElapsedTime().asSeconds() < secondsLimit)
	{
		m_text.setPosition(0.f, 1000-speed*textTimer.getElapsedTime().asSeconds());
		window.clear();
		print(window);
		window.display();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
}

