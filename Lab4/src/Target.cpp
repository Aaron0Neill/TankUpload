#include "Target.h"

Target::Target(sf::Texture& t_texture) : 
	m_currentPosition{0}
{
	m_targetSprite.setTexture(t_texture);
	m_targetSprite.setTextureRect({ 100,100,50,50 });
}

void Target::addPositions(std::vector<sf::Vector2f> t_pos)
{
	m_positions = t_pos;
	m_targetSprite.setPosition(m_positions[m_currentPosition]); // update the target sprite
}

void Target::onHit()
{
	m_currentPosition++; // increase the current position
	m_currentPosition %= static_cast<int>(m_positions.size()); // mod it by the size of the array so it cant go outside the bounds
	m_targetSprite.setPosition(m_positions[m_currentPosition]); // update the target sprite
}

void Target::draw(sf::RenderTarget& t_target, sf::RenderStates t_state) const
{
	t_target.draw(m_targetSprite);
}