#include "Projectile.h"
#include <RigidBody.h>
#include <SpriteRenderer.h>
#include <BoxCollider.h>
#include <DebugBox.h>
#include <Transform.h>
#include <GlobalEvents.h>
#include <ScreenWrapper.h>
#include <Physics.h>
#include <ObjectPool.h>
#include <Timer.h>
#include <HealthComponent.h>

Projectile::Projectile()
{
	m_CurrentTimer->TimerFinished.AddListener(this, std::bind(&Projectile::LifeTimeEnded, this, std::placeholders::_1, std::placeholders::_2));

	SetActive(false);
	AddComponent<RigidBody>()->setGravityEnabled(false)->SetMaxSpeed(1500.f)->SetDrag(0.f)->setDoesBounce(true);
	AddComponent<SpriteRenderer>()->setUV(LLGP::Vector2i(10, 2))->setRenderLayer(1);

	AddComponent<ScreenWrapper>();

	AddComponent<BoxCollider>()->SetUpCollider(LLGP::Vector2f(8, 8), LLGP::Vector2f(0, 0))->SetIsTrigger(true)->SetCollisionMask(0b00010111)->SetCollisionLayer(0b00001000);
	//AddComponent<DebugBox>()->SetUpDebugBox();
	GetTransform()->SetPosition(LLGP::Vector2f(0, -1000));

	SetTag("Projectile");
	SetName("Projectile");
	onCollisionEnter.AddListener(this, std::bind(&Projectile::ProjectileCollision, this, std::placeholders::_1));
	g_OnFixedUpdate.AddListener(this, std::bind(&Projectile::FixedUpdate, this, std::placeholders::_1));
}

Projectile::~Projectile()
{
	onCollisionEnter.RemoveListener(this, std::bind(&Projectile::ProjectileCollision, this, std::placeholders::_1));

	g_OnFixedUpdate.RemoveListener(this, std::bind(&Projectile::FixedUpdate, this, std::placeholders::_1));
}

void Projectile::FixedUpdate(float deltaTime)
{
}

void Projectile::ProjectileCollision(CollisionInfo* col)
{
	if (!col->otherCollider->GetGameObject()->GetActive() || !GetActive() || col->otherCollider->GetIsTrigger()) { return; }
	
	//Damage other collider (If they have the take damage function)
	if (HealthComponent* healthComponent = col->otherCollider->GetGameObject()->GetComponent<HealthComponent>())
	{
		healthComponent->TakeDamage(m_Damage);
		//std::cout << "Take Damage: " << m_Damage <<  "\n";
		DisableProjectile();
	}
	m_BounceAmount--;
	if (m_BounceAmount < 0) 
	{ 
		DisableProjectile(); 
	}
}

void Projectile::EnableProjectile(LLGP::Vector2f projectileDir, LLGP::Vector2f projectileLocation, float velocity, int bounceAmount, float damage)
{
	GetTransform()->SetPosition(projectileLocation);
	GetComponent<BoxCollider>()->SetBoxPosition(projectileLocation);
	GetComponent<RigidBody>()->SetVelocity(projectileDir * velocity);
	m_BounceAmount = bounceAmount;
	m_Damage = damage;
	SetActive(true);
}

void Projectile::DisableProjectile()
{
	ObjectPooler::DeactivateObject(this);
	GetComponent<RigidBody>()->SetVelocity(LLGP::Vector2f::zero);
	transform->SetPosition(LLGP::Vector2f(0, -1000));
	SetActive(false);
}

void Projectile::LifeTimeEnded(Timer* timer, int Successful)
{
	if (Successful == true)
	{
		DisableProjectile();
	}
}

void Projectile::SetActive(bool newActive)
{
	m_Active = newActive;
	if (m_Active)
	{
		m_CurrentTimer->StartTimer(m_ProjectileLifetime);
	}
	else
	{
		
		m_CurrentTimer->ClearAndInvalidateTimer();
	}
}
