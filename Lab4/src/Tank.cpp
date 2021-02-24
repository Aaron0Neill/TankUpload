#include "Tank.h"

Tank::Tank(sf::Texture const & texture, std::vector<sf::Sprite>& wallSprites)
: m_texture(texture),
m_wallSprites(wallSprites),
m_bulletTimer(sf::seconds(0.5f))
{
	initSprites();
	m_debuffs.push_back(TankDebuff::NONE);
}

//################################################################################################################
void Tank::update(double dt, sf::RenderWindow& window, Xbox360Controller input)
{	
	handleKeyInput(window,input);
	double radians = m_rotation * MathUtility::DEG_TO_RAD; // change the rotation into radians
	double newX = m_tankBase.getPosition().x + cos(radians) * m_speed * (dt / 1000);
	double newY = m_tankBase.getPosition().y + sin(radians) * m_speed * (dt / 1000);
	m_previousPosition = m_tankBase.getPosition(); // update the tanks previous position just before its changed
	// update the tanks position and rotation
	m_tankBase.setPosition(newX, newY);
	m_turret.setPosition(newX, newY);
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_turretRotation);
	m_speed *= 0.99; // friction
	if (m_speed > -0.5f && m_speed < 0.5f) // stop it if its moving slow
		m_speed = 0;

	if (m_centring)
		centreTurret();
	m_maxReverseSpeed = -75.0f;
	m_maxForwardSpeed = 100.0f;
	for (TankDebuff debuff : m_debuffs)
	{
		if (debuff == TankDebuff::SLOW_SPEED)
		{
			m_maxReverseSpeed = -18.75f;
			m_maxForwardSpeed = 50.0f;
		}
	}

	m_speed = std::clamp(m_speed, m_maxReverseSpeed, m_maxForwardSpeed);
	if (checkWallCollision())
	{
		deflect();
	}
	
	m_bullets.update(dt);
	m_bulletEffect.update(sf::seconds(static_cast<float>(dt)));

	if (checkBulletCollision())
		m_bulletsHit++;

	if (m_bulletTimer > sf::seconds(0))
		m_bulletTimer -= sf::seconds(static_cast<float>((dt / 1000)));
	else
		m_firing = false;
}

//################################################################################################################
void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	m_bullets.render(window);
	window.draw(m_bulletEffect);
	window.draw(m_turret);
}

//################################################################################################################
void Tank::setPosition(sf::Vector2f const& t_pos)
{
	m_tankBase.setPosition(t_pos);
	m_turret.setPosition(t_pos);
}

//################################################################################################################
void Tank::handleKeyInput(sf::RenderWindow& window, Xbox360Controller input)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		increaseSpeed();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		decreaseSpeed();
	}

	if (input.m_currentState.leftThumbStick.y != 0)
	{
		controllerSpeed(input.m_currentState.leftThumbStick.y);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		decreaseRotation();
	}
	if (input.m_currentState.leftThumbStick.x != 0)
	{
		controllerRotation(input.m_currentState.leftThumbStick.x);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		increaseRotation();
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		m_usingMouse = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		m_usingMouse = false;

	if (m_enableRotation && !m_usingMouse)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			rotateTurret(m_rotationSpeed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			rotateTurret(-m_rotationSpeed);
		}
		if (input.m_currentState.rightThumbStick.x != 0 || input.m_currentState.rightThumbStick.y != 0)
		{
			controllerTurretRotate(input.m_currentState.rightThumbStick);
		}
	}
	else if (m_enableRotation && m_usingMouse)
	{
		rotateToMouse(window);
	}

	// ** FIRING DISABLED FOR PROJECT ** 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left) || input.m_currentState.rightTrigger > 0)
	{
		if (m_bulletTimer <= sf::seconds(0))
		{
			m_bullets.create(m_texture, m_tankBase.getPosition().x, m_tankBase.getPosition().y, m_turretRotation);
			m_bulletTimer = sf::seconds(1.f);
			m_firing = true;
			m_bulletsFired++;
			initParticles();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		m_centring = true;
	}
	if (!m_enableRotation) 
		m_enableRotation = true;
}

//################################################################################################################
void Tank::initParticles()
{
	//sf::Time life = sf::seconds(5.f);
	//gunSmoke.setParticleTextureIndex(15U);
	//gunSmoke.setEmissionRate(30.f);
	//gunSmoke.setParticleLifetime(life);
	//gunSmoke.setParticlePosition(sf::Vector2f{ 100,100 });
	//gunSmoke.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f{ 100,100 }, 10.f));
	//m_bulletEffect.addEmitter(thor::refEmitter(gunSmoke));
}

//################################################################################################################
void Tank::rotateTurret(float t_rotation)
{
	m_previousTurretRotation = m_turretRotation;
	if (!m_centring)
	{
		m_turretRotation += t_rotation;
		if (m_turretRotation >= 360)
		{
			m_turretRotation = 0;
		}
		else if (m_turretRotation < 0)
		{
			m_turretRotation = 359;
		}
	}
}

//################################################################################################################
void Tank::controllerTurretRotate(sf::Vector2f vec)
{
	m_previousRotation = m_rotation;
	if (!m_centring)
	{
		double angle = atan2f(vec.y, vec.x);
		angle *= 180 / thor::Pi;
		m_turretRotation = angle;
	}
}

//################################################################################################################
void Tank::centreTurret()
{
	if (m_turretRotation > m_rotation )
	{
		m_turretRotation -= m_rotationSpeed;
	}
	else if (m_turretRotation <= m_rotation)
	{
		m_turretRotation += m_rotationSpeed;
	}
	if (m_turretRotation - m_rotation > -m_rotationSpeed && m_turretRotation - m_rotation < m_rotationSpeed)
	{
		m_centring = false;
	}
	m_previousTurretRotation = m_turretRotation;
}

//################################################################################################################
bool Tank::checkWallCollision()
{
	for (sf::Sprite const & wall : m_wallSprites)
	{
		if (CollisionDetector::collision(m_tankBase, wall) ||
			CollisionDetector::collision(m_turret, wall))
		{
			return true;
		}
	}
	return false;
}

//################################################################################################################
bool Tank::checkBulletCollision()
{
	for (sf::Sprite& wall : m_wallSprites)
	{ // check for collision between bullets and the wall
		if(m_bullets.checkCollision(wall))
			return false;
	}
	return false;
}

bool Tank::checkTargetCollision(sf::Sprite t_targetSprite)
{
	if (CollisionDetector::collision(m_tankBase, t_targetSprite))
	{
		m_score += 5;
		return true;
	}
	return false;
}

bool Tank::checkEnemyTankCollision(std::pair<sf::Sprite, sf::Sprite> t_aiTank)
{
	if (m_bullets.checkCollision(t_aiTank.first) || m_bullets.checkCollision(t_aiTank.second))
	{
		return true;
	}
	return false;
}

//################################################################################################################
void Tank::deflect()
{
	// In case tank was rotating.
	adjustRotation();

	// If tank was moving.
	if (m_speed != 0)
	{
		// Temporarily disable turret rotations on collision.
		m_enableRotation = false;
		// Back up to position in previous frame.
		m_tankBase.setPosition(m_previousPosition);
		// Apply small force in opposite direction of travel.
		if (m_previousSpeed < 0)
		{
			m_speed = 8;
		}
		else
		{
			m_speed = -8;
		}
	}
}

//################################################################################################################
void Tank::addWall(std::vector<sf::Sprite>& wallSprites)
{
	m_wallSprites = wallSprites;
}

void Tank::takeDamage(int t_damage, TankDebuff t_debuff)
{
	m_health -= t_damage;
	bool haveDebuff{ false };
	for (TankDebuff debuff : m_debuffs)
	{
		if (debuff == t_debuff)
		{
			haveDebuff = true;
		}
	}
	if (!haveDebuff)
	{
		m_debuffs.push_back(t_debuff);
	}
}

//################################################################################################################
void Tank::adjustRotation()
{
	//if tank was rotating
	if (m_rotation != m_previousRotation)
	{
		// work out which direction to rotate the tank base post-Collision
		if (m_rotation > m_previousRotation)
		{
			m_rotation = m_previousRotation - 1;
		}
		else
		{
			m_rotation = m_previousRotation + 1;
		}
		// if turret was rotating while tank was moving
		if (m_turretRotation != m_previousTurretRotation)
		{
			// set the turret rotation while tank was moving
			m_turretRotation = m_previousTurretRotation;
		}
	}
}

//################################################################################################################
void Tank::increaseSpeed()
{
	if (m_speed < 100.0)
	{
		m_speed += 1;
	}
}

//################################################################################################################
void Tank::controllerSpeed(float scalar)
{
	m_speed += 1 * (-scalar / 100.0);
}

//################################################################################################################
void Tank::decreaseSpeed()
{
	if (m_speed > -75.0) // moves slightly slower in reverse
	{
		m_speed -= 1;
	}
}

//################################################################################################################
void Tank::increaseRotation()
{
	double rotationSpeed;
	m_previousRotation = m_rotation;
	m_previousTurretRotation = m_turretRotation;

	rotationSpeed = m_rotationSpeed;
	for (TankDebuff debuffs : m_debuffs)
	{
		if (debuffs == TankDebuff::SLOW_TURN)
		{
			rotationSpeed = m_debuffRotationSpeed;
		}
	}
	m_turretRotation += rotationSpeed;
	m_rotation += rotationSpeed;
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

//################################################################################################################
void Tank::controllerRotation(float scalar)
{
	m_previousRotation = m_rotation;
	m_previousTurretRotation = m_turretRotation;
	m_turretRotation += 1 * (scalar / 100.0);
	m_rotation += 1 * (scalar / 100.0);
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

//################################################################################################################
void Tank::decreaseRotation()
{
	double rotationSpeed;
	m_previousRotation = m_rotation;
	m_previousTurretRotation = m_turretRotation;
	rotationSpeed = m_rotationSpeed;
	for (TankDebuff debuffs : m_debuffs)
	{
		if (debuffs == TankDebuff::SLOW_TURN)
		{
			rotationSpeed = m_debuffRotationSpeed;
		}
	}
	m_turretRotation -= rotationSpeed;
	m_rotation -= rotationSpeed;
	if (m_rotation == 0.0)		
	{
		m_rotation = 359.0;
	}
}

void Tank::reset()
{
	m_health = 10;
}

//################################################################################################################
void Tank::rotateToMouse(sf::RenderWindow& window)
{
	m_previousTurretRotation = m_turretRotation;
	sf::Vector2f vectorToMouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - m_turret.getPosition(); // get the distant vector from the tank to the mouse
	double newRotation = static_cast<double>(atan2f(vectorToMouse.y, vectorToMouse.x)); // get the the angle between the tank and the mouse
	newRotation *= MathUtility::RAD_TO_DEG; // turn it from degree to radians
	m_turretRotation = newRotation;

}

//################################################################################################################
int Tank::getAccuracy()
{
	if (m_bulletsFired > 0)
	{
		return ((m_bulletsHit / static_cast<double>(m_bulletsFired)) * 100);
	}
	return 0;
}

//################################################################################################################
void Tank::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);

	// load the tank shell effect	
	if (!m_smokeTexture.loadFromFile("resources//images//BulletCollisionEffect.png"))
	{
		std::cout << "Particle error\n";
	}
	//setup for particles
	m_bulletEffect.setTexture(m_smokeTexture);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			m_bulletEffect.addTextureRect(sf::IntRect{ j * 128, i * 128, 128,128 });
		}
	
}