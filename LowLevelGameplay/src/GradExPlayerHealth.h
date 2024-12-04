#pragma once
#include <Component.h>

class GradExPlayerHealth : public Component
{
public:
	GradExPlayerHealth(GameObject* owner);
	~GradExPlayerHealth();

	void OnAliveStateChanged(int dead);
};
