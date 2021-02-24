#ifndef TARGET_H
#define TARGET_H	

#include <SFML/Graphics.hpp>
#include <vector>

class Target : 
	public sf::Drawable
{
public:
	Target(sf::Texture& t_texture);

	/// <summary>
	/// @brief add a vector of positons 
	/// ** Cant be done in constructor because of the level loader ** 
	/// </summary>
	/// <param name="t_pos"> positions the target can spawn </param>
	void addPositions(std::vector<sf::Vector2f> t_pos);
	
	/// <summary>
	/// @brief function that is called when the target is hit
	/// Moves the target to another location
	/// </summary>
	void onHit();

	/// <summary>
	/// @brief allows other classes to access the target sprite but not change it
	/// </summary>
	/// <returns> the target sprite </returns>
	sf::Sprite& const getSprite() { return m_targetSprite; }

	/// <summary>
	/// BASE CLASS FUNCTION
	/// Allows classes the use of window.draw(Target)
	/// </summary>
	/// <param name="t_target"> where to draw the target </param>
	/// <param name="t_state"> what state to draw the target </param>
	void draw(sf::RenderTarget& t_target, sf::RenderStates t_state = sf::RenderStates::Default) const override;
private:
	std::vector<sf::Vector2f> m_positions;
	sf::Sprite m_targetSprite;
	int m_currentPosition;
};

#endif // !TARGET_H