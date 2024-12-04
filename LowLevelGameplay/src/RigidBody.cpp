#include "RigidBody.h"
#include "GameObject.h"
#include "Transform.h"
#include <GlobalEvents.h>
#include <Physics.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

RigidBody::RigidBody(GameObject* owner) : Component(owner)
{
	m_GameObject->onCollisionEnter.AddListener(this, std::bind(&RigidBody::OnCollisionEnter, this, std::placeholders::_1));
	m_GameObject->onCollisionExit.AddListener(this, std::bind(&RigidBody::OnCollisionExit, this, std::placeholders::_1));
	m_GameObject->onCollisionStay.AddListener(this, std::bind(&RigidBody::OnCollisionEnter, this, std::placeholders::_1));

	g_OnPhysicsUpdate.AddListener(this, std::bind(&RigidBody::FixedUpdate, this, std::placeholders::_1));
	Physics::RegisterRigidBody(this);

	g_OnFixedUpdate.RemoveListener(this, std::bind(&RigidBody::FixedUpdate, this, std::placeholders::_1));
	g_OnUpdate.RemoveListener(this, std::bind(&Component::Update, this, std::placeholders::_1));
	g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
}

RigidBody::~RigidBody()
{
	Physics::DeregisterRigidBody(this);
	m_GameObject->onCollisionStay.RemoveListener(this, std::bind(&RigidBody::OnCollisionEnter, this, std::placeholders::_1));
	m_GameObject->onCollisionEnter.RemoveListener(this, std::bind(&RigidBody::OnCollisionEnter, this, std::placeholders::_1));
	m_GameObject->onCollisionExit.RemoveListener(this, std::bind(&RigidBody::OnCollisionExit, this, std::placeholders::_1));
	g_OnPhysicsUpdate.RemoveListener(this, std::bind(&RigidBody::FixedUpdate, this, std::placeholders::_1));
}
;

void RigidBody::FixedUpdate(float deltaTime)
{
	if (!m_GameObject->GetActive()) { return; }

	LLGP::Vector2f solvedForce = SolveForces(deltaTime);
	AddVelocity(solvedForce);

	int velSignX = (GetVelocity().x >= 0) ? 1 : -1;
	int velSignY = (GetVelocity().y >= 0) ? 1 : -1;

	if (GetVelocity().x > GetMaxSpeed() || -(GetVelocity().x) > GetMaxSpeed())
	{
		m_Velocity.x = GetMaxSpeed() * velSignX;
	}
	if (GetVelocity().y > GetMaxSpeed() || -(GetVelocity().y) > GetMaxSpeed())
	{
		m_Velocity.y = GetMaxSpeed() * velSignY;
	}

	//Fix jitter
	if (GetVelocity().x < 1 && GetVelocity().x > -1)
	{
		SetVelocity(LLGP::Vector2f(0, GetVelocity().y));
	}
	if (GetVelocity().y < 1 && GetVelocity().y > -1)
	{
		SetVelocity(LLGP::Vector2f(GetVelocity().x, 0));
	}

	LLGP::Vector2f oldPos = m_GameObject->GetTransform()->ReturnPosition();

	LLGP::Vector2f newPosition;

	newPosition = (GetVelocity()* deltaTime) + (m_GameObject->GetTransform()->ReturnPosition());

	//std::cout << newPosition.x << ", " << newPosition.y << std::endl;

	LLGP::Vector2f distance = newPosition - oldPos;

	setDistanceTravelled(distance.GetMagnitude());

	m_GameObject->GetTransform()->SetPosition(newPosition);
}

void RigidBody::OnCollisionEnter(CollisionInfo* col)
{
	if (!m_GameObject->GetActive()) { return; }


	if (GetDoesBounce())
	{
		if (col->otherCollider->GetIsTrigger()) { return; }
		if (col->Normal.x != 0)
		{
			float impulseVel = abs(GetVelocity().x) * col->Normal.x * 1;
			SetVelocity(LLGP::Vector2f(impulseVel, GetVelocity().y));
		}

		if (col->Normal.y != 0)
		{
			float impulseVel = abs(GetVelocity().y) * col->Normal.y * 1;
			SetVelocity(LLGP::Vector2f(GetVelocity().x, impulseVel));
		}
	}
	else
	{
		if (col->otherCollider->GetIsTrigger() || col->collider->GetIsTrigger()) { return; }
		if (col->Normal.x != 0)
		{
			SetVelocity(LLGP::Vector2f(0, GetVelocity().y));
		}
		if (col->Normal.y != 0)
		{
			SetVelocity(LLGP::Vector2f(GetVelocity().x, 0));
		}
		OverrideNetForce(LLGP::Vector2f::zero);
	}

	AddPosition(col->Normal * std::round(col->Overlap) + col->Normal /*+ col->Normal*/);
}

void RigidBody::OnCollisionExit(CollisionInfo* col)
{
	SetIsGrounded(false);
}

void RigidBody::addForce(LLGP::Vector2f force)
{
	if (!m_GameObject->GetActive()) { return; }
	AddNetForce(force);
}

LLGP::Vector2f RigidBody::GetTotalForces() 
{
	LLGP::Vector2f currentTotalForce;
	currentTotalForce += GetNetForce();

	return LLGP::Vector2f::zero;
}

LLGP::Vector2f RigidBody::SolveForces(float deltaTime)
{
	LLGP::Vector2f velocityToAdd = LLGP::Vector2f::zero;

	if (GravityIsEnabled() && !IsGrounded())
	{
		m_NetForce += GetGravityForce();
	}
	if (IsGrounded())
	{
		m_NetForce += CalculateFrictionForce(deltaTime);
	}
	else
	{
		m_NetForce += CalculateDragForce(deltaTime);
	}
	
	velocityToAdd = (m_NetForce / GetMass()) * deltaTime;

	m_NetForce = LLGP::Vector2f::zero;

	return velocityToAdd;
}

LLGP::Vector2f RigidBody::CalculateFrictionForce(float deltaTime)
{
	//If friction is unneeded, return
	if (!IsGrounded() || GetVelocity().x == 0)
	{
		return LLGP::Vector2<float>::zero;
	}

	float returnForce = 0;

	returnForce = CalculateOpposingForce(deltaTime, GetVelocity().x, GetFriction());

	//Return friction in the X axis
	return LLGP::Vector2f(returnForce, 0);
}

float RigidBody::CalculateOpposingForce(float deltaTime, float velocityToOppose, float opposingForceStrength)
{
	float attemptedForce = 0;
	float returnForce = 0;

	// A = V/dT therefore:
	// F = V/dT * M
	float velocityForce = ((velocityToOppose / deltaTime) * GetMass());

	//Make sure the friction force is in the correct direction
	if (velocityForce > 0)
	{
		attemptedForce = -opposingForceStrength;
	}
	else if (velocityForce < 0)
	{
		attemptedForce = opposingForceStrength;
	}

	if ((velocityForce > 0 && velocityForce - attemptedForce < 0) || (velocityForce < 0 && velocityForce + attemptedForce > 0))
	{
		returnForce = -velocityForce;
	}
	else
	{
		returnForce = attemptedForce;
	}

	if (returnForce == 1000) 
	{ 
		int i = 0;
	}

	return returnForce;
}

LLGP::Vector2f RigidBody::CalculateDragForce(float deltaTime)
{
	//If Drag is unneeded, return.
	if (IsGrounded() || GetVelocity().GetSquareMagnitude() == 0)
	{
		return LLGP::Vector2f::zero;
	}
	LLGP::Vector2f DragForce = LLGP::Vector2f::zero;

	float returnForceX = CalculateOpposingForce(deltaTime, GetVelocity().x, GetDrag());
	float returnForceY = CalculateOpposingForce(deltaTime, GetVelocity().y, GetDrag());

	float test = m_Velocity.GetSquareMagnitude();

	DragForce *= (test * test);

	DragForce = LLGP::Vector2f(returnForceX, returnForceY);

	return DragForce;
}

void RigidBody::AddPosition(LLGP::Vector2f posToAdd)
{
	Transform2D* transform = m_GameObject->GetTransform();

	transform->SetPosition(LLGP::Vector2f((transform->ReturnPosition().x), (transform->ReturnPosition().y)) + posToAdd);
}
