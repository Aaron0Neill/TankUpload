#include "TitleScreen.h"

TitleScreen::TitleScreen(Game& game, sf::Font& t_font, sf::RenderWindow& t_window, sf::Sprite* b) :
	m_game(&game),
	m_font(t_font),
	m_window(&t_window),
	m_highScoreInfo(FileIO::getHighScore()),
	m_backGround(b)
{
	// stop drawing the mouse cursor and dont let the mouse move off screen 
	//m_window->setMouseCursorVisible(false);
	//m_window->setMouseCursorGrabbed(true);
	//m_window->requestFocus();

	//load all info on title screen
	loadSprite();
	initButtons();
	initHighScore();
}

//################################################################################################################

void TitleScreen::update(sf::Time t_dt)
{
	//update the cursor position
	if (!m_usingController)
		m_crosshairPos = { static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_window)) };
	else
		sf::Mouse::setPosition(static_cast<sf::Vector2i>(m_crosshairPos), *m_window);

	m_gameCursor.setPosition(m_crosshairPos);

	updateSelector(t_dt.asSeconds());

	for (int i = 0; i < m_selector.size(); i++)
	{
		m_selector[i].setRotation(m_selectorRotation[i]);
	}
}

//################################################################################################################

void TitleScreen::updateSelector(double dt)
{
	for (int i = 0; i < m_selector.size(); i++)
	{
		if (m_selection[i])
		{// if the mouse is on the text object
			if (m_selectorRotation[i] < 90)
				m_selectorRotation[i] += 6;
		}
		else if (m_selectorRotation[i] > -90)
		{
			m_selectorRotation[i] -= 6;
		}
	}
}

//################################################################################################################

void TitleScreen::render(sf::RenderWindow& t_window)
{
	m_window->clear();
	m_window->draw(*m_backGround);
	//m_window->draw(m_gameCursor);

	for (int i =0; i <  m_buttonText.size(); i++)
	{
		m_window->draw(m_button[i]);
		m_window->draw(m_buttonText[i]);
		m_window->draw(m_selector[i]);
	}
	m_window->draw(m_highScoreTitle);
	for (int i = 0; i < m_highScoreNames.size(); i++)
	{
		m_window->draw(m_highScore[i]);
		m_window->draw(m_highScoreNames[i]);
		m_window->draw(m_highScoreAcc[i]);
	}

	m_window->display();
}

//################################################################################################################

void TitleScreen::processInput(sf::Event& t_event)
{
	if (t_event.type == sf::Event::KeyPressed)
	{
		if (t_event.key.code == sf::Keyboard::Escape)
		{
			m_window->close();
		}
	}
	if (t_event.type == sf::Event::MouseButtonPressed)
	{// check to see if the mouse is on any of the buttons
		for (int i = 0; i < m_buttonText.size(); i++)
		{
			if (m_usingController) // if the player clicks the mouse hes not using a controller
				m_usingController = false;
			float w = m_buttonOutline[i].getGlobalBounds().width;
			float h = m_buttonOutline[i].getGlobalBounds().height;
			sf::Vector2f buttonCorner = { m_buttonOutline[i].getPosition().x,
			m_buttonOutline[i].getPosition().y };
			if (m_crosshairPos.x > buttonCorner.x &&
				m_crosshairPos.x < buttonCorner.x + w &&
				m_crosshairPos.y > buttonCorner.y &&
				m_crosshairPos.y < buttonCorner.y + h)
			{
				switch (i)
				{
				case 0:
					m_window->setMouseCursorVisible(true);
					m_window->setMouseCursorGrabbed(false);
					m_game->m_screenState = ScreenState::GameScreen;
					break;
				case 1:
					m_game->m_screenState = ScreenState::BuilderScreen;
					break;
				case 2:
					m_game->m_screenState = ScreenState::OptionScreen;
					break;
				case 3:
					m_window->close();
					break;
				}
			}
		}
	}
	if (t_event.type == sf::Event::MouseMoved)
	{ // check if the mouse is over any of the buttons
		for (int i = 0; i < m_buttonText.size(); i++)
		{
			float w = m_buttonOutline[i].getGlobalBounds().width;
			float h = m_buttonOutline[i].getGlobalBounds().height;
			sf::Vector2f buttonCorner = { m_buttonOutline[i].getPosition().x,
			m_buttonOutline[i].getPosition().y };
			if (m_crosshairPos.x > buttonCorner.x&&
				m_crosshairPos.x < buttonCorner.x + w &&
				m_crosshairPos.y > buttonCorner.y&&
				m_crosshairPos.y < buttonCorner.y + h)
			{
				m_selection[i] = true;
			}
			else
				m_selection[i] = false;
		}
	}
}

//#######################################################################################################

void TitleScreen::processController(sf::Time dt, Xbox360Controller input)
{
	// check if the input has moved, if it has move the mouse 
	if (input.m_currentState.leftThumbStick.x != 0 || input.m_currentState.leftThumbStick.y != 0)
	{
		if (!m_usingController)
			m_usingController = true;
		m_crosshairPos.x += (input.m_currentState.leftThumbStick.x) * m_crosshairSpeed.x / 100.f;
		m_crosshairPos.y += (input.m_currentState.leftThumbStick.y) * m_crosshairSpeed.y / 100.f;
	}
	if (input.m_currentState.a)
	{
		for (int i = 0; i < m_buttonText.size(); i++)
		{
			if (!m_usingController) // if the player clicks the A button hes using the mouseS
				m_usingController = true;
			float w = m_buttonOutline[i].getGlobalBounds().width;
			float h = m_buttonOutline[i].getGlobalBounds().height;
			sf::Vector2f buttonCorner = { m_buttonOutline[i].getPosition().x,
			m_buttonOutline[i].getPosition().y };
			if (m_crosshairPos.x > buttonCorner.x&&
				m_crosshairPos.x < buttonCorner.x + w &&
				m_crosshairPos.y > buttonCorner.y&&
				m_crosshairPos.y < buttonCorner.y + h)
			{
				switch (i)
				{
				case 0:
					m_window->setMouseCursorVisible(true);
					m_window->setMouseCursorGrabbed(false);
					m_game->m_screenState = ScreenState::GameScreen;
					break; 
				case 1:
					m_game->m_screenState = ScreenState::BuilderScreen;
					break;
				case 2:
					m_game->m_screenState = ScreenState::OptionScreen;
					break;
				case 3:
					m_window->close();
					break;
				}
			}
		}
	}
}

//#######################################################################################################
// INIT
//#######################################################################################################

void TitleScreen::initButtons()
{
	//init text
	sf::Vector2f bv2f{ 100,100 };
	for (auto& b : m_buttonText)
	{
		b.setCharacterSize(40);
		b.setFont(m_font);
		b.setPosition(bv2f);
		bv2f.y += 100;
	}
	m_buttonText[0].setString("Play");
	m_buttonText[1].setString("Build");
	m_buttonText[2].setString("Options");
	m_buttonText[3].setString("Exit");

	//init Circles
	for (int i =0; i < m_button.size(); i++)
	{
		m_button[i].setRadius(12.5f); // make the button the same size as the text
		m_button[i].setFillColor(sf::Color::White);
		m_button[i].setOrigin({ m_button[i].getRadius(),m_button[i].getRadius() });
		m_button[i].setPosition(75, m_buttonText[i].getPosition().y + (m_button[i].getRadius() * 2));
	}

	for (int i=0;i < m_buttonOutline.size(); i++)
	{
		m_buttonOutline[i].setSize({ m_buttonText[i].getGlobalBounds().width + 10, m_buttonText[i].getGlobalBounds().height + 10 });
		m_buttonOutline[i].setPosition({ m_buttonText[i].getPosition().x - 3, m_buttonText[i].getPosition().y + 4 });
		m_buttonOutline[i].setFillColor(sf::Color(155, 155, 155, 0));
		m_buttonOutline[i].setOutlineThickness(3.f);
		m_buttonOutline[i].setOutlineColor(sf::Color(100, 100, 100, 255));
	}

	// set the default for all the buttons to -90degree
	for (auto& r : m_selectorRotation)
	{
		r = -90;
	}

	// setup a little notch in the buttons
	for (int i = 0; i < m_selector.size(); i++)
	{
		m_selector[i].setPointCount(3);
		m_selector[i].setRadius(m_button[i].getRadius());
		m_selector[i].setOrigin({ m_selector[i].getRadius() ,	m_selector[i].getRadius() });
		m_selector[i].setPosition(m_button[i].getPosition());
		m_selector[i].setFillColor(sf::Color(100, 100, 100, 255));
		m_selector[i].setRotation(m_selectorRotation[i]);
	}
}

//#######################################################################################################

void TitleScreen::initHighScore()
{
	m_highScoreTitle.setFont(m_font);
	m_highScoreTitle.setCharacterSize(50);
	m_highScoreTitle.setString("High Scores");
	sf::FloatRect size = m_highScoreTitle.getGlobalBounds();
	m_highScoreTitle.setOrigin(size.width/2.f, 0);
	m_highScoreTitle.setPosition(sf::Vector2f{ 1000.f,0 });
	// setup all the text files
	sf::Vector2f namePos{ 750.f ,size.height };
	sf::Vector2f scorePos{ 1000.f, size.height };
	sf::Vector2f accuracyPos{ 1250.f, size.height };
	for (int i = 0; i < m_highScoreInfo.size() + 1; i++)
	{
		m_highScore[i].setFont(m_font);
		m_highScoreAcc[i].setFont(m_font);
		m_highScoreNames[i].setFont(m_font); 
		if (i == 0)
		{
			// setup the first element of the array to be headings for the rest of the text
			m_highScoreNames[i].setString("Names");
			m_highScore[i].setString("Scores");
			m_highScoreAcc[i].setString("Accuracy");
			m_highScoreNames[i].setFillColor(sf::Color::Yellow);
			m_highScoreNames[i].setStyle(sf::Text::Style::Italic);
			m_highScore[i].setFillColor(sf::Color::Yellow);
			m_highScore[i].setStyle(sf::Text::Style::Italic);
			m_highScoreAcc[i].setFillColor(sf::Color::Yellow);
			m_highScoreAcc[i].setStyle(sf::Text::Style::Italic);
		}
		else
		{
			// set all the other elements to be the actual information
			m_highScoreNames[i].setString(m_highScoreInfo[i-1].m_playerName);
			m_highScore[i].setString(std::to_string(m_highScoreInfo[i - 1].m_score));
			m_highScoreAcc[i].setString(std::to_string(m_highScoreInfo[i - 1].m_accuracy));
		}
		m_highScore[i].setPosition(scorePos);
		m_highScoreAcc[i].setPosition(accuracyPos);
		m_highScoreNames[i].setPosition(namePos);
		scorePos.y += 30;
		namePos.y += 30;
		accuracyPos.y += 30;
	}
}

//################################################################################################################

void TitleScreen::loadSprite()
{
	try
	{
		// try load files necessary for the title screen
		if (!m_gameCursorTexture.loadFromFile("resources\\images\\Reticle.png"))
			throw std::exception("Error Loading Crosshair");
		if (!m_musicBuffer.loadFromFile("resources\\music\\MainMenuSong.ogg"))
			throw std::exception("Error loading Music");
	}
	catch (std::exception & e)
	{
		// if a file doesnt load stop the program
		std::cout << "ERROR: " << e.what() << std::endl;
		throw e;
	}
	// initialize the cursor
	//m_gameCursor.setTexture(m_gameCursorTexture);
	//m_gameCursor.setOrigin(m_gameCursorTexture.getSize().x / 2.f,
	//	m_gameCursorTexture.getSize().y / 2.f);
	//m_gameCursor.setScale(0.5f, 0.5f);
	//m_gameCursor.setColor(sf::Color::Blue);
	// assign the music and start playing
	m_music.setBuffer(m_musicBuffer);
	//m_music.play();
}
