#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <sstream>
#include "EntityPlacer.h"
#include "Board.h"
#include "Menu.h"
#include "About.h"
#include "EntityTimer.h"

//==== CONSTANTS ==========
const char MOVE_LIMIT_MESSAGE[] = "Moves: %d / %d";
const char TIME_LIMIT_MESSAGE[] = "Seconds: %d / %d";
const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH = 1900;

//==== DECLARATIONS ========
static bool startMenu();
static bool playGame();
static void initGameLimits(int& secondsLimit, int& movesLimit, const Board& board);

//==== FUNCTIONS ========
int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	return startMenu() ? EXIT_SUCCESS : EXIT_FAILURE;
}


//=====================================================
/*
The main function of the Paint running, it gets the size of the board
And runs the board and the objects on it
*/
static bool startMenu() {
	MenuAction action = MENU_INVALID;
	//It will use as the size of the board , when we want to reset the game
	Menu menu{};
	//Setting up the general screen of the paint which will be max size of the screen
	sf::RenderWindow window(sf::VideoMode(1900, 1080), "");
	window.clear();
	sf::Event event;
	sf::Music music;//Getting the background music
	if (!music.openFromFile("MainTitle.ogg"))
		return false;
	music.setLoop(true);
	music.play();
	while (window.isOpen() && action != MENU_START)
	{
		menu.drawMenu(window);//drawing to the screen the menu buttons
		window.display();

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			default:
				break;
			
			//Checking which button was pressed
			case sf::Event::MouseButtonPressed:
				action = menu.getButtons().whichIsPressed(event);
		
				if (action == MENU_ABOUT)
				{
					About about;
					about.runAbout();						
				}
				//if we pressed on start game
				if (action == MENU_START)
				{
					music.stop();//stop the backgroung music
					playGame();//begging to play
					action = MENU_INVALID;
					music.play();
				}
				if (action == MENU_EXIT)
				{
					window.close();
				}
				break;

			case sf::Event::Closed:
				window.close();
				break;
			}
			window.display();
		}
	}
	return true;
}
//==============================================================
//Playing the game function
//==============================================================
static bool playGame()
{

	sf::Music music;
	if (!music.openFromFile("Imperial_March.ogg"))
		return false;
	sf::Text gameText;//will be used when the player didn't win the game
	sf::Font font;
	sf::Text levelLostText;
	sf::Text gameWonText;
	sf::Color color(sf::Color::Red);
	if (!font.loadFromFile("font.otf"))
	{
		return false;
	}
	//Setting up all the texts in the game
	gameText.setFont(font);
	levelLostText.setFont(font);
	levelLostText.setFillColor(sf::Color::Red);
	levelLostText.setString("You lost the level...\n\n Press any Key to restart or e to exit game");
	levelLostText.setPosition(630, 400);
	gameWonText.setFont(font);
	gameWonText.setFillColor(sf::Color::Green);
	gameWonText.setString("You won the game...\n\n Press Esc to go back to the menu ");
	gameWonText.setPosition(630, 400);


	Board board;
	//Checking if we could load the level and the hash table of the sprites
	if (!board.init())
		return false;
	int level = board.getLevel();
	sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "");
	sf::Clock entityTimer;
	int moveCounter = 0;
	sf::Clock gameTimer;
	gameTimer.restart();
	int secondsLimit = -1;
	int movesLimit = -1;
	bool isPlaying = true;
	bool musicPause = false;
	//Getting the game limits - seconds or moves
	initGameLimits(secondsLimit, movesLimit, board);
	music.setLoop(true);
	music.play();
	while (gameWindow.isOpen())
	{
		sf::Event event;
		while (gameWindow.pollEvent(event))
		{
			if (musicPause) {
				music.play();
				musicPause = false;
			}
			switch (event.type)
			{
			default:
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Escape)
					gameWindow.close();
				else if (event.key.code == sf::Keyboard::R)
					isPlaying = false;
				else if (entityTimer.getElapsedTime() > ENTITY_TICK && isPlaying)
				{
					if (board.handleKey(event))
					{
						entityTimer.restart();
						moveCounter++;
					}
					int newLevel = board.getLevel();
					//Checking if the player finished the level
					if (newLevel != level)
					{
						level = newLevel;
						initGameLimits(secondsLimit, movesLimit, board);
						moveCounter = 0;
						gameTimer.restart();
						isPlaying = false;
					}

				}
				//Renewing the level
				if (!isPlaying)
				{
					music.stop();
					musicPause = true;
					initGameLimits(secondsLimit, movesLimit, board);
					isPlaying = true;
					gameTimer.restart();
					moveCounter = 0;
					board.restartLevel();
				}
				break;
			case sf::Event::MouseButtonPressed:
				
				if (entityTimer.getElapsedTime() > ENTITY_TICK && isPlaying)
				{
					entityTimer.restart();
					board.handleClick(event);
				}
				//Renewing the level
				if (!isPlaying)
				{
					music.stop();
					musicPause = true;
					initGameLimits(secondsLimit, movesLimit, board);
					isPlaying = true;
					gameTimer.restart();
					moveCounter = 0;
					board.restartLevel();
				}
				break;
			case sf::Event::Closed:
				gameWindow.close();
				break;
			}
		}
		gameWindow.clear(sf::Color::Black);
		int levelnew = board.getLevel();
		if (board.didWin()) {
	
			gameWindow.clear();
			gameWindow.draw(gameWonText);
		}
		board.print(gameWindow, isPlaying);

		char *mesg = NULL;
		char movesMsg[sizeof(MOVE_LIMIT_MESSAGE) / sizeof(char) + 4];
		char timeMsg[sizeof(TIME_LIMIT_MESSAGE) / sizeof(char) + 4];
		if (movesLimit > 0)
		{
			sprintf_s(movesMsg, MOVE_LIMIT_MESSAGE, moveCounter, movesLimit);
			mesg = movesMsg;
		}
		else if (secondsLimit > 0)
		{
			sprintf_s(timeMsg, TIME_LIMIT_MESSAGE, (int)gameTimer.getElapsedTime().asSeconds(), secondsLimit);
			mesg = timeMsg;
		}
		gameText.setString(mesg);
		gameText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 20*strlen(mesg) / 2, WINDOW_HEIGHT - 200));
		gameWindow.draw(gameText);
		
		bool timeOver = secondsLimit > 0 && gameTimer.getElapsedTime().asSeconds() >= secondsLimit;
		bool movesOver = movesLimit > 0 && moveCounter >= movesLimit;
		if (timeOver || movesOver)
		{
			isPlaying = false;
			//print game over message
			gameWindow.draw(levelLostText);
		}
		gameWindow.display();

	}
	return true;
}
//===================================================================
//Making the game limits randomly - as moves or seconds.
//===================================================================
static void initGameLimits(int& secondsLimit, int& movesLimit, const Board& board)
{
	//Algorythem to find the limits
	int approximateRequiredMoves = (int)(4 * sqrt(board.getBoardSize()));
	int randomMoveLimit = rand() % ((int)(approximateRequiredMoves*1.25f)) +
		(int)(0.75f*approximateRequiredMoves);

	secondsLimit = movesLimit = -1;

	//choose whether time or moves based limitation
	if (rand() % 2) //time based
	{
		secondsLimit =  randomMoveLimit*ENTITY_TICK.asSeconds() + randomMoveLimit / 2;
	}
	else //moves based
	{
		movesLimit =  randomMoveLimit;
	}
}
