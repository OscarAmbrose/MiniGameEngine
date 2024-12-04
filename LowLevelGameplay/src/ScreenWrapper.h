#pragma once
#include <Component.h>

class BoxCollider;

class ScreenWrapper : public Component
{
public:
	ScreenWrapper(GameObject* owner);
	~ScreenWrapper();

	void FixedUpdate(float deltaTime);
protected:
	float minimumScreenBounds = 0;
	float maximumScreenBounds = 1920;
	float maximumScreenBoundsY = 1200;
	BoxCollider* boxCollider;
};
