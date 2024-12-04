#include "Cursor.h"
#include <SpriteRenderer.h>
#include <GameObject.h>
#include <Transform.h>
#include <WindowManager.h>
#include <BoxCollider.h>
#include <DebugBox.h>
#include <GradExPlayer.h>
#include <Physics.h>

Cursor::Cursor(GameObject* owner) : Component(owner)
{
	m_ControllerNumber = static_cast<PlayerCharacter*>(m_GameObject)->GetPlayerNumber();
	m_JoystickDir = LLGP::Vector2f::zero;

	m_PlayerPos = m_GameObject->GetTransform()->ReturnPosition();
	m_CursorPos = m_GameObject->GetTransform()->ReturnPosition();

	m_RenderWindow = WindowManager::GetActiveWindow();
	m_RenderWindow->sf::Window::setMouseCursorVisible(false);

	m_SpriteRenderer = m_GameObject->AddComponent<SpriteRenderer>()->setUV(LLGP::Vector2i(13, 2), LLGP::Vector2i(64, 64))->setRenderLayer(4);

	m_BoxCollider = m_GameObject->AddComponent<BoxCollider>()->SetUpCollider(LLGP::Vector2f(64, 64), LLGP::Vector2f(48, 48));
	m_BoxCollider->SetCollisionMask(0b10000010)->SetCollisionLayer(0b01000000)->SetIsTrigger(true);
	
	//m_DebugBoxCollider = m_GameObject->AddComponent<DebugBox>()->SetUpDebugBox(m_BoxCollider);

	g_OnPollInputs.AddListener(this, std::bind(&Cursor::PollInput, this, std::placeholders::_1));

	m_GameObject->onCollisionEnter.AddListener(this, std::bind(&Cursor::OnCollisionEnter, this, std::placeholders::_1));
	m_GameObject->onCollisionExit.AddListener(this, std::bind(&Cursor::OnCollisionExit, this, std::placeholders::_1));
}

Cursor::~Cursor()
{
	g_OnPollInputs.RemoveListener(this, std::bind(&Cursor::PollInput, this, std::placeholders::_1));	
	m_GameObject->onCollisionEnter.RemoveListener(this, std::bind(&Cursor::OnCollisionEnter, this, std::placeholders::_1));
	m_GameObject->onCollisionExit.RemoveListener(this, std::bind(&Cursor::OnCollisionExit, this, std::placeholders::_1));
}

void Cursor::PollInput(sf::Event event)
{
	if (!GetGameObject()) { return; }

	switch (event.type)
	{
	case sf::Event::JoystickMoved:
	{
		if (!sf::Joystick::isConnected(m_ControllerNumber)) { return; }

		float axisZ = (sf::Joystick::getAxisPosition(m_ControllerNumber, sf::Joystick::U) / 100);
		float axisR = (sf::Joystick::getAxisPosition(m_ControllerNumber, sf::Joystick::V) / 100);

		if ((axisR < m_JoystickDeadzone && axisR > -m_JoystickDeadzone))
		{
			axisR = 0;
		}
		if ((axisZ < m_JoystickDeadzone && axisZ > -m_JoystickDeadzone))
		{
			axisZ = 0;
		}

		m_JoystickDir = LLGP::Vector2f(axisZ, axisR);
		//std::cout << m_JoystickDir.x << ", " << m_JoystickDir.y << std::endl;
	}
	break;
	case sf::Event::JoystickDisconnected:
	{
		std::cout << "Controller disconnected: Id: " << event.joystickMove.joystickId << std::endl;
	}
	break;
	case sf::Event::JoystickConnected:
	{
		std::cout << "Controller connected: Id: " << event.joystickMove.joystickId << std::endl;
	}
	break;
	default:
	{
		break;
	}
	}
}

void Cursor::SetCursorPosition(LLGP::Vector2f location)
{
	auto CursorPosition = m_CursorPos - m_PlayerPos;
	auto test = FixCursorPosition(m_CursorPos - m_PlayerPos);

	if (m_DebugBoxCollider)
	{
		m_DebugBoxCollider->SetOffset(test);
	}
	m_BoxCollider->SetOffset(test);
	m_SpriteRenderer->setOffSet(test);
}

LLGP::Vector2f Cursor::FixCursorPosition(LLGP::Vector2f position)
{
	if (position.x > (1920 - m_PlayerPos.x))
	{
		position.x = 1920.f - m_PlayerPos.x;
		m_CursorPos.x = 1920.f;

	}
	else if (position.x < (0 - m_PlayerPos.x))
	{
		position.x = 0 - m_PlayerPos.x;
		m_CursorPos.x = 0;

	}
	if (position.y < (0 - m_PlayerPos.y))
	{
		position.y = 0 - m_PlayerPos.y;
		m_CursorPos.y = 0;
	}
	else if (position.y > (1080 - m_PlayerPos.y))
	{
		position.y = 1080 - m_PlayerPos.y;
		m_CursorPos.y = 1080;
	}
	return position;
}

void Cursor::Update(float deltaTime)
{
	Component::Update(deltaTime);
	if (!m_GameObject->GetActive()) { return; }

	m_PlayerPos = m_GameObject->GetTransform()->ReturnPosition();

	m_CursorPos += m_JoystickDir * m_CursorMoveSpeed * deltaTime;
	SetCursorPosition(m_CursorPos);
}

LLGP::Vector2f Cursor::GetLookAtCursorDirection()
{
	return LLGP::Vector2f((m_CursorPos - m_PlayerPos).Normalised());
}

void Cursor::OnCollisionEnter(CollisionInfo* col)
{
	if (col->collider != m_BoxCollider && col->otherCollider != m_BoxCollider) { return; }

	m_CursorMoveSpeed = m_BaseCursorMoveSpeed * 0.5f;
	std::cout << m_CursorMoveSpeed << std::endl;
}

void Cursor::OnCollisionExit(CollisionInfo* col)
{
	if (col->collider != m_BoxCollider && col->otherCollider != m_BoxCollider) { return; }

	m_CursorMoveSpeed = m_BaseCursorMoveSpeed;
	std::cout << m_CursorMoveSpeed << std::endl;
}


