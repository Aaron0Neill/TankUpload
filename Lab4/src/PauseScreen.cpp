#include "PauseScreen.h"

PauseScreen::PauseScreen(Game& game, sf::Font& t, sf::RenderWindow& w) : 
	m_window(&w),
	m_game(&game)
{
	for (auto& n : m_options)
	{
		n.setFont(t);
		n.setFillColor(sf::Color::Black);
		n.setString("Paused");
		n.setPosition(720 - n.getGlobalBounds().width /2.f, 100);
		n.setCharacterSize(50U);
	}
	for (auto& opt : m_optionsSelected)
	{
		opt = false;
	}
	m_screenBlur.setFillColor(sf::Color(100, 100, 100, 100));
	m_screenBlur.setSize(sf::Vector2f{ 1440,900 });
}

bool PauseScreen::processEvent(sf::Event& t_event)
{
	if (t_event.type == sf::Event::KeyPressed)
	{
		if (t_event.key.code == sf::Keyboard::Escape)
		{
			return true;
		}
	}
	return false;
}

void PauseScreen::render()
{
	m_window->draw(m_screenBlur);

	for (auto& t : m_options)
	{
		m_window->draw(t);
	}
}
