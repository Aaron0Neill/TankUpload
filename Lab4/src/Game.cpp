/// <summary>
/// @author Aaron
/// @date 16/9/19
/// </summary>

#include "Game.h"
#include <iostream>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1440U, 900U, 32U }, "Tank Game" },
	m_exitGame{ false }, //when true game will exit
	m_tank(m_tankTexutre, m_wallSprites),
	m_aiTank(m_tankTexutre, m_wallSprites),
	m_gameTimer(sf::seconds(60.f)),
	m_hud(m_font),
	m_screenState(ScreenState::TitleScreen),
	m_currentState(GameState::INGAME),
	m_playerName(""),
	m_resetTimer(sf::seconds(5.f))	,
	m_target(m_tankTexutre)
{
	int m_currentLevel = 1;	//default to the first level
	m_window.setVerticalSyncEnabled(true);
	// will generate an exception if the level fails to load
	try
	{
		LevelLoader::load(m_currentLevel, m_level);
	}
	catch (std::exception& e)
	{
		std::cout << "Level loading failure." << std::endl;
		std::cout << e.what() << std::endl;
		throw e;
	}

	m_target.addPositions(m_level.m_target.m_position);

	m_aiTank.addWaypoints(m_level.m_tankAI.m_waypoints);

	setup();
	m_aiTank.init(m_level.m_tankAI.m_position);
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
	delete m_pauseScreen;
	delete m_titleScreen;
}


//################################################################################################################
/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

//################################################################################################################
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		switch (m_screenState)
		{
		case ScreenState::TitleScreen:
			m_titleScreen->processInput(newEvent);
			break;
		case ScreenState::GameScreen:
			if (m_currentState == GameState::INGAME)
			{
				if (sf::Event::KeyPressed == newEvent.type)
				{
					if (sf::Keyboard::Escape == newEvent.key.code)
					{
						m_currentState = GameState::PAUSED;
					}
#ifdef _DEBUG
					//testing spawns when in debug mode
					if ((sf::Keyboard::Tab) == newEvent.key.code)
					{
						restart();
					}
#endif
				}
			}
			else if (m_currentState == GameState::SAVING)
			{
				saveScore(newEvent);
			}
			else if (m_currentState == GameState::PAUSED)
			{
				if (m_pauseScreen->processEvent(newEvent))
					m_currentState = GameState::INGAME;
			}
			if (sf::Event::Closed == newEvent.type) // window message
			{
				m_exitGame = true;
			}
			break;
		case ScreenState::BuilderScreen:
			break;
		default:
			break;
		}
	}
}

//################################################################################################################
/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	m_input.update();
	switch (m_screenState)
	{
	case ScreenState::TitleScreen:
		m_titleScreen->update(t_deltaTime);
		if (m_input.isConnect())
		{
			m_titleScreen->processController(t_deltaTime, m_input);
		}
		break;
	case ScreenState::GameScreen:
		m_hud.update(m_currentState, m_tank.getHealth(), m_tank.getScore(), m_targetsHit, m_tank.getStatus());
		switch (m_currentState)
		{
		case GameState::INGAME:
			// NOT NEEDED FOR THE PROJECT
			if (m_tank.getBulletTime().asSeconds() > 0.8f)
			{// check if the player has fired and if so shake the screen 
				shakeScreen(m_tank.getBulletTime());
			}
			else
			{
				m_gameView.setRotation(0.f);
				m_gameView.setCenter(720.f, 450.f);
			}

			if (m_aiTank.checkBulletCollision(m_tank))
			{
				m_tank.takeDamage(m_aiTank.getDamage(), static_cast<TankDebuff>(rand() % 3));

				if (m_tank.getHealth() <= 0)
				{
					m_currentState = GameState::GAME_LOSE;
				}
			}

			if (m_tank.checkTargetCollision(m_target.getSprite()))
			{
				m_targetsHit++;
				if (m_targetsHit >= m_totalTargets)
				{
					m_currentState = GameState::GAME_WIN;
				}
				else
				{
					m_target.onHit();
				}
			}

			if (m_tank.checkEnemyTankCollision(m_aiTank.getTank()))
			{
				m_aiTank.hitByBullet(1);
				if (m_aiTank.getHealth() <= 0)
				{
					m_currentState = GameState::GAME_WIN;
				}
			}

			if (m_aiTank.collidesWithPlayer(m_tank))
			{
				m_currentState = GameState::GAME_LOSE;
			}

			m_tank.update(t_deltaTime.asMilliseconds(), m_window, m_input);

			m_aiTank.update(m_tank, t_deltaTime.asMilliseconds());

			//if (m_gameTimer > sf::seconds(0.f)) // if the game hasnt been running until the timer, add to the timer
			//	m_gameTimer -= sf::seconds(static_cast<float>(t_deltaTime.asSeconds()));
			//else // if it has been running long enough, end the game.
			//	m_currentState = GameState::SAVING;

			break;
		case GameState::PAUSED:
			break;
		case GameState::SAVING:
			m_showName.setString("Enter your name: " + m_playerName);
			break;
		case GameState::GAME_WIN:
			if (m_resetTimer.asSeconds() > 0.f)
			{// reset the game
				m_resetTimer -= t_deltaTime;
			}
			else
			{
				restart();
			}
			break;
		case GameState::GAME_LOSE:
			if (m_resetTimer.asSeconds() > 0.f)
			{// reset the game
				m_resetTimer -= t_deltaTime;
			}
			else
			{
				restart();
			}
			break;
		default:
			break;
		}
		
		
		break;
	case ScreenState::BuilderScreen:
		break;
	default:
		break;
	}
	
}

//################################################################################################################
/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.setView(m_gameView);
	switch (m_screenState)
	{
	case ScreenState::TitleScreen:
		m_titleScreen->render(m_window);
		break;
	case ScreenState::GameScreen:
		m_window.clear();
		m_window.draw(m_bgSprite);
		for (auto& m_wallSprites : m_wallSprites)
		{
			m_window.draw(m_wallSprites);
		}

		m_window.draw(m_target);

		m_tank.render(m_window);

		m_aiTank.render(m_window);
		
		m_hud.render(m_window);

		if (m_currentState == GameState::PAUSED)
			m_pauseScreen->render();

		if (m_currentState == GameState::SAVING)
			m_window.draw(m_showName);

		m_window.display();
		break;
	case ScreenState::BuilderScreen:
		break;
	default:
		break;
	}
}

//################################################################################################################
void Game::setup()
{
	//load files
	try
	{
		if (!m_tankTexutre.loadFromFile(m_level.m_tank.m_fileName))
		{
			throw std::exception("Error Loading SpriteSheet");
		}
		if (!m_spriteSheetTexture.loadFromFile(m_level.m_tank.m_fileName))
		{
			throw std::exception("Error Loading SpriteSheet");
		}
		if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
		{
			throw std::exception("Error Loading background");
		}
		if (!m_font.loadFromFile("resources\\font\\ariblk.ttf"))
		{
			throw std::exception("Error Loading Font");
		}
		if (!m_targetTexture.loadFromFile(m_level.m_target.m_fileName))
		{
			throw std::exception("Error loading target file\n");
		}
	}
	catch (std::exception error)
	{
		std::cout << "Loading error\n";
		std::cout << error.what();
		throw error;
	}
	//setup view
	m_gameView.setCenter(720.f, 450.f);
	m_gameView.setSize(1440.f, 900.f);

	//assign background 
	m_bgTexture.setRepeated(true);
	m_bgSprite.setTexture(m_bgTexture);
	m_bgSprite.setTextureRect(sf::IntRect{ 0, 0, 1500, 1000 });
	m_bgSprite.setPosition(-10.f, -10.f);
	//assign wall
	sf::Sprite sprite;
	sf::IntRect	wallRect(2, 129, 33, 23);
	sprite.setTexture(m_spriteSheetTexture);
	sprite.setTextureRect(wallRect);
	//Loop through each obstacle instance
	for (auto& Obstacle : m_level.m_obstacles)
	{
		// assign data from the levelData
		sprite.setOrigin(wallRect.width / 2.f, wallRect.height / 2.f);
		sprite.setPosition(static_cast<sf::Vector2f>(Obstacle.m_position));
		sprite.setRotation(static_cast<float>(Obstacle.m_rotation));
		m_wallSprites.push_back(sprite);
	}
	m_tank.addWall(m_wallSprites);
	m_tank.setPosition(m_level.m_tank.m_position[rand() % 4]);


	m_titleScreen = new TitleScreen(*this, m_font, m_window, &m_bgSprite);
	m_pauseScreen = new PauseScreen(*this, m_font, m_window);

	m_showName.setFont(m_font);
	m_showName.setCharacterSize(40);

	m_input.connect();
}

//################################################################################################################
/// <summary>
/// function to make the tank respawn in one of the locations in the yaml file
/// #For testing spawns only
/// </summary>
void Game::restart()
{
	m_tank.setPosition(m_level.m_tank.m_position[rand() % 4]);
	m_playerName = "";
	m_tank.reset();
	m_currentState = GameState::INGAME;
	m_aiTank.reset();
	m_resetTimer = sf::seconds(5.f);
}

//################################################################################################################

void Game::saveScore(sf::Event& t_event)
{
	bool caps{ false };
	if (t_event.type == sf::Event::KeyPressed)
	{
		if (m_playerName.length() < 20) //make sure it fits on the screen
		{
			if (sf::Keyboard::LShift == t_event.key.code)
			{ // check to see if you're pressing shift
				caps = true;
			}
			if ((t_event.key.code >= 0 && t_event.key.code <= 25)) //check between a-z
			{
				char letter;
				if (caps)
					letter = (t_event.key.code + 65);
				else
					letter = (t_event.key.code + 97);
				m_playerName += letter;
			}
			else if (t_event.key.code == 57) //space
			{
				m_playerName += " ";
			}
			if (t_event.key.code == 58) //enter
			{
				FileIO::Stats playerStats(m_tank.getScore(), m_playerName, m_tank.getAccuracy());

				FileIO saveFile(playerStats);

				m_window.close();
			}
			else if (t_event.key.code == 59) //backspace
			{
				unsigned length = m_playerName.length();
				if (length > 0)
					m_playerName.erase(length - 1, length);
			}
		}
		if (sf::Event::KeyReleased == t_event.type)
		{
			if (sf::Keyboard::LShift == t_event.key.code)
			{
				caps = false;
			}
		}
	}

}

//################################################################################################################

void Game::shakeScreen(sf::Time shakeTime)
{
	m_gameView.setRotation(rand() % 4 - 2);
	m_gameView.move(sf::Vector2f{ static_cast<float>(rand() % 4 - 2), static_cast<float>(rand() % 4 - 2) });
}
