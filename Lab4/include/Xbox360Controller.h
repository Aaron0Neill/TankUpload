/// <summary>
/// @Author: Aaron O Neill
/// @Date: 16-9-2019
/// </summary>
#ifndef XBOX360CONTROLLER
#define XBOX360CONTROLLER

#include <SFML/Graphics.hpp>
#include <iostream>

// keeps track of what buttons have been pressed on the controller
struct ControllerState
{
	bool a{ false };
	bool b{ false };
	bool x{ false };
	bool y{ false };
	bool lb{ false };
	bool rb{ false };
	bool leftThumbStickClick{ false };
	bool rightThumbStickClick{ false };
	bool dpadUp{ false };
	bool dpadDown{ false };
	bool dpadLeft{ false };
	bool dpadRight{ false };
	bool start{ false };
	bool back{ false };
	bool xBoxLogo{ false };
	float rightTrigger{ false };
	float leftTrigger{ false };
	sf::Vector2f rightThumbStick;
	sf::Vector2f leftThumbStick;
};

/// <summary>
/// Class that accepts input from an xbox controller
/// </summary>
class Xbox360Controller
{
private:
	int const c_DPAD_DEADZONE{ 50 }; // controls how far you have to move the dPad before it registers
	int const c_JOYSTICK_DEADZONE{ 15 }; // controls how far you have to move the joystick before it registers

public:

	static int s_noOfControllers; // keeps track of how many controllers are connected

	ControllerState m_currentState; // keeps track of what buttons are being pressed
	ControllerState m_previousState; // keeps track of the state the controller was in

	int m_controllerNumber{ 0 }; // keeps track of which number this controller is

	Xbox360Controller();
	~Xbox360Controller();

	void update();
	void dPad();
	void trigger();
	bool isConnect();
	bool connect();
	inline float vectorLength(sf::Vector2f t_pos) { return sqrt((t_pos.x * t_pos.x) + (t_pos.y * t_pos.y)); };
};

#endif