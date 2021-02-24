/// <summary>
/// @Author: Aaron O Neill
/// <summary>
///@author: Aaron O Neill
/// </summary>
#include "Xbox360Controller.h"

int Xbox360Controller::s_noOfControllers = 0;

Xbox360Controller::Xbox360Controller()
{
}

Xbox360Controller::~Xbox360Controller()
{
}

/// <summary>
/// Main update function for the controller
/// </summary>
void Xbox360Controller::update()
{
	if (sf::Joystick::isButtonPressed(m_controllerNumber, 0)) // A is pressed
	{
		if (!m_previousState.a)
		{
			m_currentState.a = true;
		}
	}
	else
	{
		m_currentState.a = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 1)) // B is pressed
	{
		if (!m_previousState.b)
		{
			m_currentState.b = true;
		}
	}
	else
	{
		m_currentState.b = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 2)) // x is pressed
	{
		if (!m_previousState.x)
		{
			m_currentState.x = true;
		}
	}
	else
	{
		m_currentState.x = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 3)) // Y is pressed
	{
		if (!m_previousState.y)
		{
			m_currentState.y = true;
		}
	}
	else
	{
		m_currentState.y = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 4)) // LB is pressed
	{
		if (!m_previousState.lb)
		{
			m_currentState.lb = true;
		}
	}
	else
	{
		m_currentState.lb = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 5)) // RB is pressed
	{
		if (!m_previousState.rb)
		{
			m_currentState.rb = true;
		}
	}
	else
	{
		m_currentState.rb = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 6)) // back Button pressed
	{
		if (!m_previousState.back)
		{
			m_currentState.back = true;
		}
	}
	else
	{
		m_currentState.back = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 7)) // Start Button pressed
	{
		if (!m_previousState.start)
		{
			m_currentState.start = true;
		}
	}
	else
	{
		m_currentState.start = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 8)) // Left Thumb Clicked
	{
		if (!m_previousState.leftThumbStickClick)
		{
			m_currentState.leftThumbStickClick = true;
		}
	}
	else
	{
		m_currentState.leftThumbStickClick = false;
	}

	if (sf::Joystick::isButtonPressed(m_controllerNumber, 9)) // Right Button pressed
	{
		if (!m_previousState.rightThumbStickClick)
		{
			m_currentState.rightThumbStickClick = true;
		}
	}
	else
	{
		m_currentState.rightThumbStickClick = false;
	}

	dPad();
	trigger();

	m_previousState = m_currentState;

}

/// <summary>
/// Check for movement on the dPad
/// </summary>
void Xbox360Controller::dPad()
{
	// update the left thumb stick position
	sf::Vector2f thumbTest = sf::Vector2f{ sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::X),
		 sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::Y) };
	// check to see if the thumb stick has moved passed a certain 
	if (vectorLength(thumbTest) > c_JOYSTICK_DEADZONE)
	{
		m_currentState.leftThumbStick = sf::Vector2f{ sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::X),
		 sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::Y) };
	}
	else
	{
		m_currentState.leftThumbStick = sf::Vector2f{ 0,0, };
	}

	//update the right thumb stick position
	thumbTest = sf::Vector2f{ sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::U),
			sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::V) };
	
	//check to see if the joysticks moved past the deadzone
	if (vectorLength(thumbTest) > c_JOYSTICK_DEADZONE)
	{
		m_currentState.rightThumbStick = sf::Vector2f{ sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::U),
		 sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::V) };
	}
	else
	{
		m_currentState.rightThumbStick = sf::Vector2f{ 0,0 };
	}

	//update the left trigger position
	m_currentState.leftTrigger = sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::Z);

	//update the right trigger position
	m_currentState.rightTrigger = (sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::Z) * -1);

	if (m_currentState.leftTrigger < 5)
		m_currentState.leftTrigger = 0;

	if (m_currentState.rightTrigger < 5)
		m_currentState.rightTrigger = 0;
}

/// <summary>
/// Check for movement on the triggers and josticks
/// </summary>
void Xbox360Controller::trigger()
{
	//check the position of the dPad

	float dPadX = sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::PovX);
	float dPadY = sf::Joystick::getAxisPosition(m_controllerNumber, sf::Joystick::PovY);

	if (dPadX > c_DPAD_DEADZONE) // moves right
	{
		if (!m_previousState.dpadRight)
		{
			m_currentState.dpadRight = true;
		}
	}
	else
	{
		m_currentState.dpadRight = false;
	}

	if (dPadX < -c_DPAD_DEADZONE) // moves Left
	{
		if (!m_previousState.dpadLeft)
		{
			m_currentState.dpadLeft = true;
		}
	}
	else
	{
		m_currentState.dpadLeft = false;
	}

	if (dPadY < -c_DPAD_DEADZONE) // moves Down
	{
		if (!m_previousState.dpadDown)
		{
			m_currentState.dpadDown = true;
		}
	}
	else
	{
		m_currentState.dpadDown = false;
	}

	if (dPadY > c_DPAD_DEADZONE) // moves Up
	{
		if (!m_previousState.dpadUp)
		{
			m_currentState.dpadUp = true;
		}
	}
	else
	{
		m_currentState.dpadUp = false;
	}
}

bool Xbox360Controller::isConnect()
{
	return sf::Joystick::isConnected(m_controllerNumber);
}

bool Xbox360Controller::connect()
{
	//check against the max supported joysticks
	for (int i = 0; i < sf::Joystick::Count; i++)
	{
		if (sf::Joystick::isConnected(i))
		{
			m_controllerNumber = s_noOfControllers++;
			return true;
		}
	}
	//if theres no controller return false
	return false;
}
