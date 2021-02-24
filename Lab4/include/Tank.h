#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include <Thor/Vectors.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Animations.hpp>
#include <Thor/Math/Distributions.hpp>
#include <iostream>
#include "Target.h"
#include "ProjectilePool.h"
#include <math.h>
#include "Xbox360Controller.h"
#include "TankDebuff.h"

/// <summary>
/// @brief A simple tank controller.
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	Tank(sf::Texture const& texture, std::vector<sf::Sprite>& wallSprites);

	void update(double dt, sf::RenderWindow& window, Xbox360Controller input);

	void render(sf::RenderWindow & window);

	void setPosition(sf::Vector2f const& t_pos); 

	void handleKeyInput(sf::RenderWindow& window, Xbox360Controller input);

	void initParticles();

	void rotateTurret(float t_rotation);

	void controllerTurretRotate(sf::Vector2f vec);

	void centreTurret();

	bool checkWallCollision();

	bool checkBulletCollision();

	bool checkTargetCollision(sf::Sprite t_targetSprite);

	bool checkEnemyTankCollision(std::pair<sf::Sprite, sf::Sprite> t_aiTank);

	void deflect();

	void addWall(std::vector<sf::Sprite>& wallSprites);

	void takeDamage(int t_damage, TankDebuff t_debuff);

	void adjustRotation(); 

	void increaseSpeed();
	void controllerSpeed(float scalar);
	void decreaseSpeed();

	void increaseRotation();
	void controllerRotation(float scalar);
	void decreaseRotation();

	void reset();

	void rotateToMouse(sf::RenderWindow& window);
	
	int getHealth() { return m_health; }
	int getScore() { return m_score; }
	int getAccuracy();
	sf::Sprite getBase() const { return m_tankBase; }
	sf::Sprite getTurret() const { return m_turret; }
	sf::Time getBulletTime() { return m_bulletTimer; }
	sf::Vector2f getPosition() const { return m_tankBase.getPosition(); }
	std::vector<TankDebuff> getStatus() const { return m_debuffs; }

private:
	void initSprites();
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const & m_texture;
	sf::Texture m_smokeTexture;
	sf::Vector2f m_turretToMouse;
	sf::Vector2f m_previousPosition;
	sf::Vector2i m_mousePos;
	std::vector<sf::Sprite> m_wallSprites;
	// The tank speed.
	double m_speed{ 0.0 };
	double m_previousSpeed{ 0.0 };

	// The current rotation as applied to tank base.
	double m_rotation{ 0.0 };
	bool m_centring{ false };
	bool m_usingMouse{ false  };
	bool m_enableRotation{ true };
	bool m_firing{ false };
	double m_turretRotation{ 0.0 };
	double m_previousRotation{ 0.0 };
	double m_rotationSpeed{ 1.0f };
	double m_debuffRotationSpeed{ 0.5f };
	double m_previousTurretRotation{ 0.0 };
	double m_maxForwardSpeed{ 100.0f };
	double m_maxReverseSpeed{ -75.0f };
	int m_bulletsFired{ 0 };
	int m_bulletsHit{ 0 };
	int m_score{ 0 };
	int m_health{ 10 };
	ProjectilePool m_bullets;
	sf::Time m_bulletTimer;
	sf::Clock m_particleClock;
	thor::ParticleSystem m_bulletEffect;
	thor::UniversalEmitter gunSmoke;
	
	std::vector<TankDebuff> m_debuffs;
};
