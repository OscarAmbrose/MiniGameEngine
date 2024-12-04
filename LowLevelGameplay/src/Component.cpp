#include "Component.h"
#include <Transform.h>
#include <GlobalEvents.h> 
#include <GameObject.h>
#include <Physics.h>

Component::Component(GameObject* owner) : m_GameObject(owner)
{
	g_OnUpdate.AddListener(this, std::bind(&Component::Update, this, std::placeholders::_1));
	g_OnStart.AddListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
	g_OnFixedUpdate.AddListener(this, std::bind(&Component::FixedUpdate, this, std::placeholders::_1));
}

Component::~Component()
{
	g_OnUpdate.RemoveListener(this, std::bind(&Component::Update, this, std::placeholders::_1));
	g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
	g_OnFixedUpdate.RemoveListener(this, std::bind(&Component::FixedUpdate, this, std::placeholders::_1));
}

void Component::Start(float start)
{
	std::cout << this->GetGameObject()->GetName() << " - " << this << std::endl;
}

void Component::Update(float deltaTime)
{
	//Ensure Start is only called once:
	if (!m_Started)
	{
		g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
		m_Started = true;
	}

}

void Component::FixedUpdate(float deltaTime)
{
}
