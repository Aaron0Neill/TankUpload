#include "TankAi.h"

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_aiBehaviour(AiBehaviour::STOP)
	, m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
	, m_aiState(AiState::PATROL_MAP)
	, m_vision(sf::Lines)
	, m_currentWayPoint(0)
	, m_bulletTimer(sf::seconds(0))
{
	// Initialises the tank base and turret sprites.
	initSprites();
}

////////////////////////////////////////////////////////////
void TankAi::update(Tank const & playerTank, double dt)
{
	updateViewArc(playerTank.getPosition());

	sf::Vector2f targetPos = updateTarget(playerTank.getPosition());

	if (AiState::ATTACK_PLAYER == m_aiState)
	{
		m_bulletTimer -= sf::milliseconds(dt);
		if (m_bulletTimer.asSeconds() <= 0)
		{
			m_bullets.create(m_texture, m_tankBase.getPosition().x, m_tankBase.getPosition().y, m_tankBase.getRotation());
			m_bulletTimer = sf::seconds(1.0f);
		}
	}

	m_bullets.update(dt);

	m_steering += thor::unitVector(targetPos);
	m_steering += collisionAvoidance();
	m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
	m_velocity = MathUtility::truncate(m_velocity + m_steering, MAX_SPEED);
	//m_acceleration = m_steering / MASS;
	//m_velocity = MathUtility::truncate(m_velocity + m_acceleration, MAX_SPEED);

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / thor::Pi * 180 + 180;

	auto currentRotation = m_rotation;

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}

	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_rotation = static_cast<int>((m_rotation) + 1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_rotation = static_cast<int>((m_rotation) - 1) % 360;
	}

	updateMovement(dt);
}

////////////////////////////////////////////////////////////

sf::Vector2f TankAi::updateTarget(sf::Vector2f t_playerPos)
{
	if (thor::length(m_tankBase.getPosition() - m_waypoints[m_currentWayPoint]) < 25.0f)
	{
		m_currentWayPoint = (++m_currentWayPoint) % m_waypoints.size();
	}

	sf::Vector2f targetpos;
	sf::Vector2f vectorToPlayer = seek(t_playerPos);
	sf::Vector2f vectorToPatrol = seek(m_waypoints[m_currentWayPoint]);

	switch (m_aiState)
	{
	case TankAi::AiState::PATROL_MAP:
		targetpos = vectorToPatrol;
		break;
	case TankAi::AiState::ATTACK_PLAYER:
		targetpos = vectorToPlayer;
		break;
	default:
		break;
	}

	return targetpos;
}

////////////////////////////////////////////////////////////

void TankAi::render(sf::RenderWindow & window)
{
	window.draw(m_tankBase);

	m_bullets.render(window);

	window.draw(m_turret);
	window.draw(m_vision);
}

////////////////////////////////////////////////////////////

void TankAi::init(sf::Vector2f position)
{
	m_tankBase.setPosition(position);
	m_turret.setPosition(position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.5f);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		circle.setFillColor(sf::Color(255, 255, 255, 100));
		m_obstacles.push_back(circle);
	}
}

////////////////////////////////////////////////////////////

bool TankAi::collidesWithPlayer(Tank const& playerTank) const
{
	// Checks if the AI tank has collided with the player tank.
	if (CollisionDetector::collision(m_turret, playerTank.getTurret()) ||
		CollisionDetector::collision(m_tankBase, playerTank.getBase()))
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////

bool TankAi::checkBulletCollision(Tank const& playerTank)
{
	if (m_bullets.checkCollision(playerTank.getBase()))
	{
		return true;
	}
	for (sf::Sprite walls : m_wallSprites)
	{
		m_bullets.checkCollision(walls);
	}
	return false;
}

void TankAi::updateViewArc(sf::Vector2f const& position)
{
	// clear the array 
	m_vision.clear();
	
	auto headingRadians = thor::toRadian(m_rotation); // get the rotation of the tank
	sf::Vector2f headingVector(std::cos(headingRadians) * m_viewRange, std::sin(headingRadians) * m_viewRange); // get a vector for how far the tank can see in that direction
	sf::Vector2f deltaView = headingVector; // add that vector to the tank so you know how far the tank can see

	// store two duplicates of that point (rotate each one different ways to get the two arc pts)
	sf::Vector2f endPointOne = deltaView;
	sf::Vector2f endPointTwo = endPointOne;
	sf::Vector2f tankPos = m_tankBase.getPosition();

	// rotate them by the FOV of the tank
	thor::rotate(endPointOne, m_coneAngle);
	thor::rotate(endPointTwo, -m_coneAngle);

	// check if the player lies inside the cone
	sf::Vector2f vectorToPlayer = position - tankPos;
	sf::Vector2f topVisionPt = endPointOne;
	sf::Vector2f bottomVisionPt = endPointTwo;

	float crossOne = thor::crossProduct(vectorToPlayer, topVisionPt);
	float crossTwo = thor::crossProduct(vectorToPlayer, bottomVisionPt);

	// check if the player position is inside the cone of vision
	if (crossOne > 0 && crossTwo < 0)
	{
		// check if the player is within view range
		if (thor::length(vectorToPlayer) < m_viewRange)
		{
			m_aiState = AiState::ATTACK_PLAYER;
		}
		else
		{
			m_aiState = AiState::PATROL_MAP;
		}
	}

	// translate them back
	endPointOne += tankPos;
	endPointTwo += tankPos;

	// append them to the array 
	m_vision.append(sf::Vertex{ tankPos, sf::Color::Red });
	m_vision.append(sf::Vertex{ endPointOne, sf::Color::Red });

	m_vision.append(sf::Vertex{ tankPos, sf::Color::Red });
	m_vision.append(sf::Vertex{ endPointTwo, sf::Color::Red });
	
}

////////////////////////////////////////////////////////////

void TankAi::reset()
{
	m_aiState = AiState::PATROL_MAP;
	m_steering = { 0,0 };
	m_tankBase.setPosition(720.f, 400.f);
	m_turret.setPosition(720.f, 400.f);
	m_health = 3;
}

////////////////////////////////////////////////////////////

sf::Vector2f TankAi::seek(sf::Vector2f playerPosition) const
{
	// calculate the vector to the player by taking the current pos away from the 
	sf::Vector2f vec{ playerPosition - m_tankBase.getPosition() };
	return vec;
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::collisionAvoidance()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = m_tankBase.getPosition() + headingVector;
	m_halfAhead = m_tankBase.getPosition() + (headingVector * 0.5f);
	const sf::CircleShape mostThreatening = findMostThreateningObstacle();
	sf::Vector2f avoidance(0, 0);

	if (MathUtility::lineIntersectsCircle(m_ahead, m_halfAhead, mostThreatening))
	{		
		avoidance.x = m_ahead.x - mostThreatening.getPosition().x;
		avoidance.y = m_ahead.y - mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{
		avoidance *= 0.0f;
	}
	return avoidance;
}

////////////////////////////////////////////////////////////

const sf::CircleShape TankAi::findMostThreateningObstacle()
{
	sf::CircleShape mostThreatening; 
	sf::Vector2f tankPos = m_tankBase.getPosition();
	sf::Vector2f firstThreat = m_obstacles.at(0).getPosition(); // initialize the most threating to be the first element in the array 
	double distance = MathUtility::distance(firstThreat, tankPos);
	for (auto c : m_obstacles)
	{
		if (MathUtility::distance(c.getPosition(), tankPos) < distance)
		{
			distance = MathUtility::distance(c.getPosition(), tankPos);
			mostThreatening = c;
		}
	}

	return mostThreatening;
}

////////////////////////////////////////////////////////////
void TankAi::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(103, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(122, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);

}


////////////////////////////////////////////////////////////
void TankAi::updateMovement(double dt)
{
	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000),
		m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_rotation);
}