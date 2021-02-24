#pragma once
#include "MathUtility.h"
#include "Tank.h"
#include "CollisionDetector.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>
#include <queue>
#include "ProjectilePool.h"

class TankAi
{
public:
	/// <summary>
	/// @brief Constructor that stores a reference to the obstacle container.
	/// Initialises steering behaviour to seek (player) mode, sets the AI tank position and
	///  initialises the steering vector to (0,0) meaning zero force magnitude.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="wallSprites">A reference to the container of wall sprites</param>
	TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites);

	/// <summary>
	/// @brief adds a vector of points to the map that the AI tank will move to
	/// </summary>
	void addWaypoints(std::vector<sf::Vector2f> t_waypoints) { m_waypoints = t_waypoints; };

	/// <summary>
	/// @brief Steers the AI tank towards the player tank avoiding obstacles along the way.
	/// Gets a vector to the player tank and sets steering and velocity vectors towards
	/// the player if current behaviour is seek. If behaviour is stop, the velocity vector
	/// is set to 0. Then compute the correct rotation angle to point towards the player tank. 
	/// If the distance to the player tank is < MAX_SEE_AHEAD, then the behaviour is changed from seek to stop.
	/// Finally, recalculate the new position of the tank base and turret sprites.
	/// </summary>
	/// <param name="playerTank">A reference to the player tank</param>
	/// <param name="dt">update delta time</param>
	void update(Tank const & playerTank, double dt);

	/// <summary>
	/// @brief Function to update where the AI tank is moving too
	/// Controls if its close to waypoint to change waypoint
	/// also checks the current state and decides if it should attack the player
	/// </summary>
	sf::Vector2f updateTarget(sf::Vector2f t_playerPos);

	/// <summary>
	/// @brief Draws the tank base and turret.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

	/// <summary>
	/// @brief Initialises the obstacle container and sets the tank base/turret sprites to the specified position.
	/// <param name="position">An x,y position</param>
	/// </summary>
	void init(sf::Vector2f position);

	/// <summary>
	/// @brief Checks for collision between the AI and player tanks.
	///
	/// </summary>
	/// <param name="player">The player tank instance</param>
	/// <returns>True if collision detected between AI and player tanks.</returns>
	bool collidesWithPlayer(Tank const& playerTank) const;

	/// <summary>
	/// @brief checks to see if the bullets the tank shoots collide with the player
	/// return if it does or not
	/// </summary>
	/// <param name="playerTank"> player tank </param>
	bool checkBulletCollision(Tank const& playerTank);

	/// <summary>
	/// @brief updates the current view range and arc of the player 
	/// ONLY IN DEBUG
	/// </summary>
	void updateViewArc(sf::Vector2f const& position);

	void reset();

	void hitByBullet(int t_damage) { m_health -= t_damage; }

	enum class AiType
	{
		AI_ID_NONE,
		AI_ID_SEEK_SHOOT_AT_PLAYER
	};

	std::pair<sf::Sprite, sf::Sprite> getTank() { return std::pair<sf::Sprite, sf::Sprite>{m_tankBase, m_turret}; }

	int getHealth() { return m_health; }
	int getDamage() { return m_bulletDamage; }

private:
	void initSprites();

	void updateMovement(double dt);

	sf::Vector2f seek(sf::Vector2f playerPosition) const;

	sf::Vector2f collisionAvoidance();

	const sf::CircleShape findMostThreateningObstacle();

	// A reference to the sprite sheet texture.
	sf::Texture const & m_texture;

	// A sprite for the tank base.
	sf::Sprite m_tankBase;

	// A sprite for the turret
	sf::Sprite m_turret;

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite> & m_wallSprites;

	// The current rotation as applied to tank base and turret.
	double m_rotation{ 0.0 };

	// Current velocity.
	sf::Vector2f m_velocity;

	// Steering vector.
	sf::Vector2f m_steering;

	// The ahead vector.
	sf::Vector2f m_ahead;

	// The half-ahead vector.
	sf::Vector2f m_halfAhead;

	sf::VertexArray m_vision;

	std::vector<sf::Vector2f> m_waypoints;

	ProjectilePool m_bullets;
	int m_bulletDamage{ 1 };

	int m_currentWayPoint;


	// The maximum see ahead range.
	static float constexpr MAX_SEE_AHEAD{ 150.0f };

	static float constexpr m_viewRange{ 500.0f }; // how far the tank can see 
	static float constexpr m_coneAngle{ 22.5f }; // how wide the view arc on the tank is 

	static float constexpr FIRING_RANGE{ 200.f }; // range the tank will be at before firing 

	// The maximum avoidance turn rate.
	static float constexpr MAX_AVOID_FORCE{ 150.0f };

	// 
	static float constexpr MAX_FORCE{ 10.0f };

	// The maximum speed for this tank.
	float MAX_SPEED = 50.0f;

	const float MASS{ 1.0f };
	sf::Vector2f m_acceleration;

	sf::Time m_bulletTimer;

	int m_health{ 3 };

	// A container of circles that represent the obstacles to avoid.
	std::vector<sf::CircleShape> m_obstacles;

	enum class AiState
	{
		PATROL_MAP,
		ATTACK_PLAYER
	} m_aiState;

	enum class AiBehaviour
	{
		SEEK_PLAYER,
		STOP,
		ROAM
	} m_aiBehaviour;


};

