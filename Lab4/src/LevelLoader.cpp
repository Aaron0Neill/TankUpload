#include "LevelLoader.h"

/// The various operator >> overloads below are non-member functions used to extract
///  the game data from the YAML data structure.

/// <summary>
/// @brief Extracts the obstacle type, position and rotation values.
/// 
/// </summary>
/// <param name="obstacleNode">A YAML node</param>
/// <param name="obstacle">A simple struct to store the obstacle data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& obstacleNode, ObstacleData& obstacle)
{
	obstacle.m_type = obstacleNode["type"].as<std::string>();
	obstacle.m_position.x = obstacleNode["position"]["x"].as<float>();
	obstacle.m_position.y = obstacleNode["position"]["y"].as<float>();
	obstacle.m_rotation = obstacleNode["rotation"].as<double>();
}

/// <summary>
/// @brief Extracts the filename for the game background texture.
/// 
/// </summary>
/// <param name="backgroundNode">A YAML node</param>
/// <param name="background">A simple struct to store background related data</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& backgroundNode, BackgroundData& background)
{
	background.m_fileName = backgroundNode["file"].as<std::string>();
}

/// <summary>
/// @brief Extracts the initial screen position for the player tank.
/// 
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& tankNode, TankData& tank)
{
	tank.m_fileName = tankNode["file"].as<std::string>();
	int i = 0;
	for (YAML::Node const & node : tankNode["spawns"])
	{
		tank.m_position[i].x = node["position"]["x"].as<float>();
		tank.m_position[i].y = node["position"]["y"].as<float>();
		i++;
	}
}

void operator >> (const YAML::Node& tankNode, TankAI_Data& tankAI)
{
	tankAI.m_fileName = tankNode["file"].as<std::string>();
	tankAI.m_position.x = tankNode["position"]["x"].as<float>();
	tankAI.m_position.y = tankNode["position"]["y"].as<float>();

	for (auto& spawn : tankNode["spawns"])
	{
		tankAI.m_waypoints.push_back(sf::Vector2f{ spawn["position"]["x"].as<float>(),
			spawn["position"]["y"].as<float>() });
	}
}

void operator >> (const YAML::Node& targetNode, TargetData& target)
{
	target.m_fileName = targetNode["file"].as<std::string>();
	
	for (auto& spawn : targetNode["spawns"])
	{
		target.m_position.push_back(sf::Vector2f{ spawn["position"]["x"].as<float>(),
			spawn["position"]["y"].as<float>() });
		target.m_offset.push_back(spawn["position"]["offset"].as<float>());
	}
}

/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& levelNode, LevelData& level)
{
	levelNode["background"] >> level.m_background;

	levelNode["tank"] >> level.m_tank;

	levelNode["target"] >> level.m_target;

	levelNode["ai_tank"] >> level.m_tankAI;

	const YAML::Node& obstaclesNode = levelNode["obstacles"].as<YAML::Node>();

	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		ObstacleData obstacle;
		obstaclesNode[i] >> obstacle;
		level.m_obstacles.push_back(obstacle);
	}
}

////////////////////////////////////////////////////////////
void LevelLoader::load(int nr, LevelData& level)
{
	std::stringstream ss;
	ss << ".//resources//levels//level";
	ss << nr;
	ss << ".yaml";

	

	try
	{
		YAML::Node baseNode = YAML::LoadFile(ss.str());
		if (baseNode.IsNull())
		{
			std::string message("File: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> level;
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}
}


