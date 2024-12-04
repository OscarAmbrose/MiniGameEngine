#include "BoxCollider.h"
#include "GameObject.h"
#include <Transform.h>
#include <Physics.h>
#include <DebugBox.h>
#include <GlobalEvents.h>
#include <bitset>

BoxCollider::BoxCollider(GameObject* owner) : Collider(owner)
{
	//SetUpCollider(LLGP::Vector2f(26, 38), LLGP::Vector2f(-7, -9));
	SetCollisionLayer(0b00000000);
	SetCollisionMask(0b00000000);
	g_OnFixedUpdate.AddListener(this, std::bind(&BoxCollider::FixedUpdate, this, std::placeholders::_1));
}

BoxCollider::~BoxCollider()
{
	g_OnFixedUpdate.RemoveListener(this, std::bind(&BoxCollider::FixedUpdate, this, std::placeholders::_1));

}

void BoxCollider::Start(float deltaTime)
{
	if (m_DebugEnabled)
	{
		m_GameObject->AddComponent<DebugBox>()->SetUpDebugBox();
	}
}


BoxCollider* BoxCollider::SetUpCollider(LLGP::Vector2f boxSize, LLGP::Vector2f boxOffset)
{
	m_Offset = boxOffset;
	m_BoxSize = boxSize;
	m_BoxPosition = m_GameObject->GetTransform()->ReturnPosition() + GetOffset();
	m_HalfBoxExtents = LLGP::Vector2f(GetBoxSize().x/2, GetBoxSize().y/2) * m_GameObject->GetTransform()->returnScale();
	return this;
}

BoxCollider* BoxCollider::SetBoxSize(LLGP::Vector2f newBoxSize)
{
	m_BoxSize = newBoxSize;
	m_HalfBoxExtents = LLGP::Vector2f(GetBoxSize().x / 2, GetBoxSize().y / 2) * m_GameObject->GetTransform()->returnScale();
	return this;
}

BoxCollider* BoxCollider::SetDebugEnabled(bool newDebug)
{
	m_DebugEnabled = newDebug;
	return this;
}

CollisionInfo* BoxCollider::IsCollidingWith(Collider* other)
{
	return IsCollidingWith((BoxCollider*)other);
}

CollisionInfo* BoxCollider::IsCollidingWith(BoxCollider* other)
{
	return Physics::Collision_AABBAABB(this, other);
}

void BoxCollider::FixedUpdate(float deltaTime)
{	
	//Component::Update(deltaTime);
	SetBoxPosition((m_GameObject->GetTransform()->ReturnPosition() + GetOffset()));
}

