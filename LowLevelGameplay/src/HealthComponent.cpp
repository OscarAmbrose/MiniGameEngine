#include <HealthComponent.h>
#include <algorithm>

HealthComponent::HealthComponent(GameObject* owner) : Component(owner)
{

}

HealthComponent::~HealthComponent()
{
	AliveStateChanged.Empty();
}

HealthComponent* HealthComponent::SetMaxHealth(float newHealth, bool isFilling)
{
	m_MaxHealth = newHealth;
	
	if (isFilling) { m_CurrentHealth = m_MaxHealth; }

	return this;
}

void HealthComponent::SetCurrentHealth(float newHealth)
{
	m_CurrentHealth = std::clamp(newHealth, 0.f, m_MaxHealth);
	HealthChanged.Invoke(m_CurrentHealth);

	if (CheckIfDead())
	{
		Death();
	}
}

void HealthComponent::TakeDamage(float damage)
{
	SetCurrentHealth(m_CurrentHealth - damage);
}

void HealthComponent::Death()
{
	AliveStateChanged.Invoke(0);
}

void HealthComponent::Respawn()
{
	AliveStateChanged.Invoke(1);
	SetCurrentHealth(m_MaxHealth);
}

