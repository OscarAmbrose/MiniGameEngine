#pragma once
#include <Component.h>

class TestComponent : public Component
{
public:
	TestComponent(GameObject* owner) : Component(owner)
	{

	}

	~TestComponent()
	{

	}
};
