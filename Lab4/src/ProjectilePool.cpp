#include "ProjectilePool.h"

ProjectilePool::ProjectilePool()
{
}

void ProjectilePool::create(sf::Texture const& texture, double x, double y, double rotation)
{
	// If no projectiles available, simply re-use the next in sequence.
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_projectiles.at(m_nextAvailable).init(texture, x, y, rotation);
}

bool ProjectilePool::update(double dt)
{
	// The number of active projectiles.
	int activeCount = 0;
	// Assume the pool is not full initially.
	m_poolFull = false;
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		if (!m_projectiles.at(i).update(dt))
		{
			// If this projectile has expired, make it the next available.
			m_nextAvailable = i;
		}
		else
		{
			// So we know how many projectiles are active.
			activeCount++;
		}
	}
	// If no projectiles available, set a flag.
	if (s_POOL_SIZE == activeCount)
	{
		m_poolFull = true;
	}
	return true;
}

void ProjectilePool::render(sf::RenderWindow& window)
{
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If projectile is active...
		if (m_projectiles.at(i).inUse())
		{
			window.draw(m_projectiles.at(i).m_projectile);
		}
	}
}

bool ProjectilePool::checkCollision(sf::Sprite object)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles.at(i).inUse())
		{
			if (CollisionDetector::collision(m_projectiles.at(i).getBullet(), object))
			{
				m_projectiles[i].remove();
				return true;
			}
		}
	}
	return false;
}
