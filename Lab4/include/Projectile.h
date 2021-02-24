//@author: Aaron O neill
#pragma once

#include <SFML/Graphics.hpp>
#include "CollisionDetector.h"
#include "MathUtility.h"

class Projectile
{
	friend class ProjectilePool;
public:
	Projectile() = default;
	void init(sf::Texture const& texture, double xPos, double yPos, double rotation);
	bool update(double dt);
	bool inUse() const;
	void remove();
	sf::Sprite getBullet() { return m_projectile; }

private:

	bool isOnScreen(sf::Vector2f position) const;

	static constexpr double s_MAX_SPEED{ 1000.0 };
	double m_speed{ s_MAX_SPEED };
	bool m_collision{ false };
	sf::Sprite m_projectile;
	sf::IntRect m_projectileRect;
};

