#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <array>
#include "FileIO.h"

class Game; // circular inculsion block 

class TitleScreen
{
public: 
	TitleScreen(Game& game, sf::Font& t_font, sf::RenderWindow& t_window, sf::Sprite* t_backGround);
	void update(sf::Time t_dt);
	void updateSelector(double dt);
	void render(sf::RenderWindow& t_window);
	void processInput(sf::Event& t_event);
	void processController(sf::Time dt, Xbox360Controller input);

	void initButtons();
	void initHighScore();
	void loadSprite();

private:

	Game* m_game;

	sf::Vector2f m_crosshairPos; // position of the crosshair on the screen 
	bool m_usingController{ false };
	sf::Vector2f m_crosshairSpeed{ 5,5 };

	sf::Texture m_gameCursorTexture;
	sf::Sprite m_gameCursor;
	sf::Sprite* m_backGround;
	sf::RenderWindow* m_window;

	sf::Font m_font;
	sf::Text m_gameTitle;

	std::array<sf::Text, 4> m_buttonText; // text objects to hold the buttons info
	std::array<sf::RectangleShape, 4> m_buttonOutline; // objects to show around the buttons
	std::array<sf::CircleShape, 4> m_button; // button that goes next to the word
	std::array<sf::CircleShape, 4> m_selector; // little triangle in the circle that spins when highlighted
	std::array<float, 4> m_selectorRotation; // controls how far each selector has rotated
	std::array<bool, 4> m_selection; // controls if the cursor is hovering on a text object

	sf::Text m_highScoreTitle; // main High scores text
	std::array<sf::Text, 11> m_highScoreNames; // text for the names of the players on the high score table
	std::array<sf::Text, 11> m_highScore; // text to show the score of each player in the table
	std::array<sf::Text, 11> m_highScoreAcc; // text to show the accuracy of each player in the table

	sf::Vector2f m_textStartPoint; 

	std::vector<FileIO::Stats> m_highScoreInfo; // vector to hold all the info thats read in from the high score file

	// Main menu Sound
	sf::SoundBuffer m_musicBuffer;
	sf::Sound m_music;
};

