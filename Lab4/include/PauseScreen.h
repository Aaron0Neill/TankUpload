#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "Game.h"

class Game;

class PauseScreen
{
public: 
	PauseScreen(Game& game, sf::Font& t,sf::RenderWindow& w);

	bool processEvent(sf::Event& t_event);

	void render();
	
private:
	Game* m_game;
	sf::RenderWindow* m_window;
	sf::RectangleShape m_screenBlur;
	std::array<sf::Text, 3> m_options;
	std::array<bool, 3> m_optionsSelected{false};
};

