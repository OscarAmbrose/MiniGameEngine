#include "GradExPlayerController.h"
#include <GlobalEvents.h>
#include <GameObject.h>
#include <Transform.h>
#include <RigidBody.h>
#include <GradExPlayer.h>
#include <Weapon.h>

PlayerController::PlayerController(GameObject* owner) : Component(owner)
{
	m_PlayerNumber = static_cast<PlayerCharacter*>(m_GameObject)->GetPlayerNumber();
	m_JoystickDir = LLGP::Vector2f::zero;
	g_OnPollInputs.AddListener(this, std::bind(&PlayerController::PollInput, this, std::placeholders::_1));
}

PlayerController::~PlayerController()
{
	if (m_ShouldDisableWeapon)
	{
		if (Weapon* weapon = m_GameObject->GetComponent<Weapon>())
		{
			weapon->SetFiring(false);
			m_ShouldDisableWeapon = false;
		}
	}
	
	g_OnPollInputs.RemoveListener(this, std::bind(&PlayerController::PollInput, this, std::placeholders::_1));
}

void PlayerController::PollInput(sf::Event event)
{
	if (!m_GameObject->GetActive()) { ChangePlayerWeaponActive(false); return; }
	switch (event.type)
	{		
		case sf::Event::JoystickButtonPressed:
		{
			if (event.joystickButton.joystickId != m_PlayerNumber) { return; }
			if (event.joystickButton.button == 2)
			{
				Weapon* playerWeapon;
				if (playerWeapon = m_GameObject->GetComponent<Weapon>())
				{
					playerWeapon->Reload();
				}
			}
			if (event.joystickButton.button == 5)
			{
				ChangePlayerWeaponActive(true);
			}
			break;
		}
		case sf::Event::JoystickMoved:
		{
			//If the input is from the incorrect controller, return.
			if (!sf::Joystick::isConnected(m_PlayerNumber)) { return; }

			//Get both axis values (/100 to make it a float between 1 and 0)
			float axisX = (sf::Joystick::getAxisPosition(m_PlayerNumber, sf::Joystick::X) / 100);
			float axisY = (sf::Joystick::getAxisPosition(m_PlayerNumber, sf::Joystick::Y) / 100);
			
			//To fix stick drift
			if ((axisX < m_JoystickDeadzone && axisX > -m_JoystickDeadzone))
			{
				axisX = 0;
			}
			if ((axisY < m_JoystickDeadzone && axisY > -m_JoystickDeadzone))
			{
				axisY = 0;
			}

			//Set the joystick direction
			m_JoystickDir = LLGP::Vector2f(axisX, axisY);

			break;
		}
		case sf::Event::JoystickButtonReleased:
		{
			if (!sf::Joystick::isConnected(m_PlayerNumber)) { return; }
			if (event.joystickButton.button == 5)
			{
				ChangePlayerWeaponActive(false);
			}
			break;
		}



		//If the event isnt being looked for, do nothing.
		default:
			//std::cout << "Non KeyPressed event" << std::endl;
			break;
	}

}

void PlayerController::FixedUpdate(float deltaTime)
{
	//If the joystick has no direction, return.
	if (m_JoystickDir == LLGP::Vector2f::zero)
	{
		return;
	}

	//Add a large force in the direction of movement (designed to make movement snappy, not floaty)
	m_GameObject->GetComponent<RigidBody>()->addForce(m_JoystickDir * 100000.f);
}

void PlayerController::ChangePlayerWeaponActive(bool newActive)
{
	Weapon* playerWeapon;
	if (playerWeapon = m_GameObject->GetComponent<Weapon>())
	{
		//player weapon sfx can go here
		playerWeapon->SetFiring(newActive);
	}
}
