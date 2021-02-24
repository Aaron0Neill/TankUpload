#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "Projectile.h"

class ProjectilePool
{
public:


	ProjectilePool();

	void create(sf::Texture const& texture, double x, double y, double rotation);

	bool update(double dt);

	void render(sf::RenderWindow& window);

	bool checkCollision(sf::Sprite object);

private:
	static const int s_POOL_SIZE = 10;

	std::array<Projectile, s_POOL_SIZE> m_projectiles;
	int m_nextAvailable{ 0 };
	bool m_poolFull{ false };
	sf::Texture m_spriteSheet;

};