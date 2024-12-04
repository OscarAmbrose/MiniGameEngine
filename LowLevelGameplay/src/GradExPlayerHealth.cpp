#include <GradExPlayerHealth.h>
#include <GameObject.h>
#include <Transform.h>
#include <HealthComponent.h>
#include <GradExGameManager.h>
#include <RespawnManager.h>

GradExPlayerHealth::GradExPlayerHealth(GameObject* owner) : Component(owner)
{
	m_GameObject->GetComponent<HealthComponent>()->AliveStateChanged.AddListener(this, std::bind(&GradExPlayerHealth::OnAliveStateChanged, this, std::placeholders::_1));
}

GradExPlayerHealth::~GradExPlayerHealth()
{
}

/// <summary>
/// Handles the result of events from the Health Component.
/// </summary>
/// <param name="state">0 = player is dead, !0 = player is alive.</param>
void GradExPlayerHealth::OnAliveStateChanged(int state)
{
	if (state == 0) 
	{ 
		m_GameObject->SetActive(false);
		RespawnManager::PlayerDied(m_GameObject);
	}
	else 
	{ 
		m_GameObject->SetActive(true);
	}
}

