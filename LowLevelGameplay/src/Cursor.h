#pragma once
#include <Component.h>
#include <SFML/Graphics.hpp>
#include <Vector2.h>

struct CollisionInfo;
class SpriteRenderer;
class BoxCollider;
class DebugBox;

class Cursor : public Component
{
public:
	Cursor(GameObject* owner);
	~Cursor();

	inline sf::Window* GetWindow() { return m_RenderWindow; }

	void PollInput(sf::Event event);

	void SetCursorPosition(LLGP::Vector2f location);

	LLGP::Vector2f FixCursorPosition(LLGP::Vector2f position);

	void Update(float deltaTime);

	Cursor* SetControllerNumber(int newNum) { m_ControllerNumber = newNum; return this; }

	LLGP::Vector2f GetLookAtCursorDirection();

	void OnCollisionEnter(CollisionInfo* col) override;
	void OnCollisionExit(CollisionInfo* col) override;

	SpriteRenderer* m_SpriteRenderer = nullptr;
protected:
	sf::Window* m_RenderWindow = nullptr;


	LLGP::Vector2f m_JoystickDir;

	LLGP::Vector2f m_PlayerPos;
	LLGP::Vector2f m_CursorPos;

	BoxCollider* m_BoxCollider = nullptr;
	DebugBox* m_DebugBoxCollider = nullptr;

	int m_ControllerNumber;

	float m_JoystickDeadzone = 0.2f;

	float m_BaseCursorMoveSpeed = 900.f;
	float m_CursorMoveSpeed = m_BaseCursorMoveSpeed;

	
};
