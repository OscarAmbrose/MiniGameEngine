#pragma once
#include <Component.h>
#include <Event.h>
#include <Timer.h>

class HealthComponent : public Component
{
public:
	HealthComponent(GameObject* owner);
	~HealthComponent();

	HealthComponent* SetMaxHealth(float newHealth, bool isFilling);
	void SetCurrentHealth(float newHealth);

	void TakeDamage(float damage);

	bool CheckIfDead() { m_IsDead = (m_CurrentHealth <= 0); return m_IsDead; }

	void Death();

	inline float GetRespawnTimer() const { return m_RespawnTime; }
	HealthComponent* SetRespawnTimer(float newTime) { m_RespawnTime = newTime; return this; }

	inline bool DoesRespawn() const { return m_Respawns; }
	HealthComponent* SetDoesRespawn(bool newRespawns) { m_Respawns = newRespawns; return this; }

	void Respawn();

	LLGP::Event<float> HealthChanged;
	LLGP::Event<int> AliveStateChanged;
private:
	float m_MaxHealth = 100;
	float m_CurrentHealth = m_MaxHealth;
	bool m_IsDead = false;
	bool m_Respawns = false;
	float m_RespawnTime = 2.f;
};
