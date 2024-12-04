#include "ScreenWrapper.h"
#include <GameObject.h>
#include <Transform.h>
#include <BoxCollider.h>

ScreenWrapper::ScreenWrapper(GameObject* owner) : Component (owner)
{
	if (boxCollider = m_GameObject->GetComponent<BoxCollider>())
	{
		minimumScreenBounds -= boxCollider->GetBoxHalfExtents().x;
		maximumScreenBounds += boxCollider->GetBoxHalfExtents().x;
	}
	else
	{
		minimumScreenBounds -= 50.f;
		maximumScreenBounds += 50.f;
	}
}

ScreenWrapper::~ScreenWrapper()
{
}

void ScreenWrapper::FixedUpdate(float deltaTime)
{
	float PositionX = m_GameObject->GetTransform()->ReturnPosition().x;

	float newPositionX = 0;

	if (PositionX < minimumScreenBounds)
	{
		newPositionX = maximumScreenBounds;
	}
	else if (PositionX > maximumScreenBounds)
	{
		newPositionX = minimumScreenBounds;
	}
	else
	{
		newPositionX = PositionX;
	}

	if (newPositionX != PositionX)
	{
		m_GameObject->GetTransform()->SetPosition(LLGP::Vector2f(newPositionX, m_GameObject->GetTransform()->ReturnPosition().y));
	}
}
