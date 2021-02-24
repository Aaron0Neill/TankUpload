/// <summary>
/// author Aaron
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "Tank.h"
#include "TankAI.h"
#include "Target.h"
#include <vector>
#include <array>
#include "HUD.h"
#include "TitleScreen.h"
#include "PauseScreen.h"
#include "FileIO.h"
#include "GameState.h"

class PauseScreen;
class TitleScreen;

enum class ScreenState
{
	TitleScreen,
	GameScreen,
	OptionScreen,
	BuilderScreen
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();
	ScreenState m_screenState;

private:

	//##############################################
	// FUNCTIONS
	//##############################################

	/// <summary>
	/// @brief Creates the wall sprites and loads them into a vector.
	/// Note that sf::Sprite is considered a light weight class, so 
	///  storing copies (instead of pointers to sf::Sprite) in std::vector is acceptable.
	/// </summary>
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();	
	void setup();
	void restart();
	void saveScore(sf::Event& t_event);
	void shakeScreen(sf::Time shakeTime);

	//##############################################
	// Game Variables
	//##############################################
	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame;
	sf::View m_gameView;
	sf::Font m_font;
	LevelData m_level; // load the game data
	sf::Texture m_tankTexutre; // tank texture
	sf::Texture m_bgTexture; // background texture
	sf::Texture m_targetTexture; // target texture
	sf::Sprite m_bgSprite; // background sprite
	std::vector<sf::Sprite> m_wallSprites; // wall sprites
	sf::Texture m_spriteSheetTexture; // sprite sheet texture
	Tank m_tank; // instance of tank class
	TankAi m_aiTank; // ai that will search for the tank
	sf::Time m_gameTimer; // keeps track of how long the game has left

	HUD m_hud;

	std::string m_playerName;
	sf::Text m_showName;
	Xbox360Controller m_input;
	float angle{ 10 };
	sf::Time m_resetTimer;

	Target m_target;
	int m_totalTargets{ 10 };
	int m_targetsHit{ 0 };


	//##############################################
	// Saving Variables
	//##############################################

	struct Table
	{
		float m_score{ 0 };
		std::string m_name{ "" };
	};

	GameState m_currentState;
	

	//##############################################
	// Screen Variables
	//##############################################
	   	  
	TitleScreen* m_titleScreen;
	PauseScreen* m_pauseScreen;

};

#endif // !GAME_HPP

