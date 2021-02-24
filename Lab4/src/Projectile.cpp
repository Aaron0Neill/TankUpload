#include "Projectile.h"
#include <iostream>

void Projectile::init(sf::Texture const& texture, double xPos, double yPos, double rotation)
{
	m_projectile.setTexture(texture);
	m_projectileRect = { 6, 179, 10, 6 };   
	m_projectile.setTextureRect(m_projectileRect);
	m_projectile.setOrigin(m_projectileRect.width / 2.0, m_projectileRect.height / 2.0);
	m_projectile.setPosition(xPos, yPos);
	m_projectile.setRotation(rotation);
	m_speed = s_MAX_SPEED;
}

bool Projectile::update(double dt)
{
	if (!inUse())
	{
		// If this projectile is not in use, there is no update routine to perform.
		return false;
	}

	sf::Vector2f position = m_projectile.getPosition();
	sf::Vector2f newPos(position.x + std::cos(MathUtility::DEG_TO_RAD * m_projectile.getRotation()) * m_speed * (dt / 1000),
		position.y + std::sin(MathUtility::DEG_TO_RAD * m_projectile.getRotation()) * m_speed * (dt / 1000));

	m_projectile.setPosition(newPos);

	if (!isOnScreen(newPos))
	{
		m_speed = 0;
	}

	return m_speed == s_MAX_SPEED;
}

bool Projectile::inUse() const
{
	return m_speed == s_MAX_SPEED;
}

void Projectile::remove()
{
	m_speed = 0;
	m_projectile.setPosition(-100, -100);
}

bool Projectile::isOnScreen(sf::Vector2f position) const
{
	return position.x - m_projectileRect.width / 2 > 0.f
		&& position.x + m_projectileRect.width / 2 < 1440
		&& position.y - m_projectileRect.height / 2 > 0.f
		&& position.y + m_projectileRect.height / 2 < 900;
}
