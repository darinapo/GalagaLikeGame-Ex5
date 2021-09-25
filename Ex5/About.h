#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class About
{
public:
	About();
	void print(sf::RenderWindow &window);
	sf::Text getText(unsigned int index) { return m_text; };
	void runAbout();
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Font m_font;
	sf::Text m_text;
};

