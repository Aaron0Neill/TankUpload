#include "HUD.h"
//
//HUD::HUD(sf::Font& t_font)
//{
//	m_hudInfo.setFont(t_font);
//}
//
//void HUD::update(int t_timeLeft, int t_accuracy, int t_score, int t_targetTime)
//{
//	m_hudInfo.setString("Time Remaining: " + std::to_string(t_timeLeft) +
//		"\tScore: " + std::to_string(t_score) +
//		"\tAccuracy: " + std::to_string(t_accuracy) + " %\tTartget Time: " +
//		std::to_string(t_targetTime));
//}
//
//void HUD::render(sf::RenderWindow& t_window)
//{
//	t_window.draw(m_hudInfo);
//}


////////////////////////////////////////////////////////////
HUD::HUD(sf::Font& hudFont)
	: m_textFont(hudFont)
{
	m_gameStateText.setFont(hudFont);
	m_gameStateText.setCharacterSize(30);
	m_gameStateText.setFillColor(sf::Color::Blue);
	m_gameStateText.setPosition(sf::Vector2f(500, 5));
	m_gameStateText.setString("Game Running");

	//Setting up our hud properties 
	m_hudOutline.setSize(sf::Vector2f(1440.0f, 40.0f));
	m_hudOutline.setFillColor(sf::Color(0, 0, 0, 38));
	m_hudOutline.setOutlineThickness(-.5f);
	m_hudOutline.setOutlineColor(sf::Color(0, 0, 0, 100));
	m_hudOutline.setPosition(0, 0);

	m_tankStatus.setFont(hudFont);
	m_tankStatus.setCharacterSize(30);
	m_tankStatus.setFillColor(sf::Color::Blue);
	m_tankStatus.setPosition(sf::Vector2f(0, 5));
	m_tankStatus.setString("Debuffs: ");

}

////////////////////////////////////////////////////////////
void HUD::update(GameState const& gameState, int t_playerHealth, int t_score, int t_targetsHit, std::vector<TankDebuff> t_debuffs)
{
	std::string hudInfo;
	std::string debuffInfo;

	switch (gameState)
	{
	case GameState::INGAME:
		hudInfo = "Game Running";
		//m_gameStateText.setString("Game Running");
		break;
	case GameState::GAME_WIN:
		hudInfo = "You Won";
		//m_gameStateText.setString("You Won");
		break;
	case GameState::GAME_LOSE:
		hudInfo = "You Lost";
		//m_gameStateText.setString("You Lost");
		break;
	default:
		break;
	}
	hudInfo += "\t Health: " + std::to_string(t_playerHealth);
	hudInfo += "\t Score: " + std::to_string(t_score);
	hudInfo += "\t Targets: " + std::to_string(t_targetsHit) + " / 10";

	debuffInfo = "Debuffs: ";
	for (TankDebuff debuff : t_debuffs) 
	{
		debuffInfo += toString(debuff);
	}

	m_gameStateText.setString(hudInfo);
	m_tankStatus.setString(debuffInfo);
}

void HUD::render(sf::RenderWindow& window)
{
	window.draw(m_hudOutline);
	window.draw(m_gameStateText);
	window.draw(m_tankStatus);
}

std::string HUD::toString(TankDebuff t_debuff)
{
	switch (t_debuff)
	{
	case TankDebuff::NONE:
		return std::string();
		break;
	case TankDebuff::SLOW_TURN:
		return "Slow turn, ";
		break;
	case TankDebuff::SLOW_SPEED:
		return "Slow Speed, ";
		break;
	default:
		break;
	}

	return std::string();
}
