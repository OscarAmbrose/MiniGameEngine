#pragma once
#include <HealthComponent.h>

class RespawnManager
{
public:
	RespawnManager() = delete;
	static void PlayerDied(GameObject* player);
};
