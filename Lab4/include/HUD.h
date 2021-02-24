/// <summary>
/// author Aaron
/// </summary>
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameState.h"
#include "TankDebuff.h"

class HUD
{
public:
	/// <summary>
	/// @brief Default constructor that stores a font for the HUD and initialises the general HUD appearance.
	/// </summary>
	HUD(sf::Font& hudFont);

	/// <summary>
	/// @brief Checks the current game state and sets the appropriate status text on the HUD.
	/// </summary>
	/// <param name="gameState">The current game state</param>
	void update(GameState const& gameState, int t_playerHealth, int t_score, int t_targetsHit, std::vector<TankDebuff> t_debuffs);

	/// <summary>
	/// @brief Draws the HUD outline and text.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow& window);

private:

	std::string toString(TankDebuff t_debuff);

	// The font for this HUD.
	sf::Font m_textFont;

	// A container for the current HUD text.
	sf::Text m_gameStateText;

	sf::Text m_tankStatus;

	// A simple background shape for the HUD.
	sf::RectangleShape m_hudOutline;
};