#pragma once
#include <Component.h>
#include <Vector2.h>
#include <Event.h>

struct CollisionInfo;

class RigidBody : public Component
{
public:
	RigidBody(GameObject* owner);
	~RigidBody();

	void FixedUpdate(float deltaTime) override;

	void OnCollisionEnter(CollisionInfo* col);
	void OnCollisionExit(CollisionInfo* col);

	void addForce(LLGP::Vector2f force);
	LLGP::Vector2f SolveForces(float deltaTime);
	LLGP::Vector2f CalculateFrictionForce(float deltaTime);
	LLGP::Vector2f GetTotalForces();

	float CalculateOpposingForce(float deltaTime, float velocityToOppose, float opposingForceStrength);

	LLGP::Vector2f CalculateDragForce(float deltaTime);

#pragma region Interact with Member Variables

	inline LLGP::Vector2f GetGravityForce() const { return m_GravityForce; }
	inline void SetGravity(LLGP::Vector2f newGravityForce) { m_GravityForce = newGravityForce; }

	inline float GetFriction() const { return m_FrictionForce; }
	inline void SetFriction(float newFriction) { m_FrictionForce = newFriction; }

	inline float GetDrag() const { return m_DragForce; }
	RigidBody* SetDrag(float newDrag) { m_DragForce = newDrag; return this; }

	inline float GetMass() const { return m_Mass; }
	RigidBody* SetMass(float newMass) { m_Mass = newMass; return this; }

	inline void SetAcceleration(LLGP::Vector2f newAccel) { m_Acceleration = newAccel; }
	inline LLGP::Vector2f GetAcceleration() const { return m_Acceleration; }

	RigidBody* SetMaxSpeed(float newMax) { m_MaxSpeed = newMax; return this; }
	inline float GetMaxSpeed() const { return m_MaxSpeed; }

	inline void OverrideNetForce(LLGP::Vector2f netForceOverride) { m_NetForce = netForceOverride; }
	inline LLGP::Vector2f GetNetForce() const { return m_NetForce; }

	inline void SetVelocity(LLGP::Vector2f newVelocity) { m_Velocity = newVelocity; }
	inline LLGP::Vector2f AddVelocity(LLGP::Vector2f addedVelocity) { return (m_Velocity += addedVelocity); }
	inline LLGP::Vector2f GetVelocity() const { return m_Velocity; }

	inline bool IsGrounded() const { return m_isGrounded; }
	void SetIsGrounded(bool newGrounded) 
	{ 
		if (newGrounded == IsGrounded())
		{
			return;
		}
		m_isGrounded = newGrounded;
		GroundedStateChanged.Invoke(IsGrounded());
	}

	inline bool GravityIsEnabled() { return m_HasGravity; }
	RigidBody* setGravityEnabled(bool newGrav) { m_HasGravity = newGrav; return this; }

	inline bool GetDoesBounce() { return m_DoesBounce; }
	RigidBody* setDoesBounce(bool newBounce) { m_DoesBounce = newBounce; return this; }

	inline bool GetOpposingMovement() { return m_OpposingMovement; }
	inline void setOpposingMovement(bool newOpposingMovement) { m_OpposingMovement = newOpposingMovement; }

	inline float GetDistanceTravelled() { return m_distanceTravelled; }
	inline void setDistanceTravelled(float newDistanceTravelled) { m_distanceTravelled = newDistanceTravelled; }

	LLGP::Event<bool> GroundedStateChanged;

#pragma endregion

protected:
	inline LLGP::Vector2f AddNetForce(LLGP::Vector2f netForceToAdd) {return (m_NetForce += netForceToAdd); }

private:
	LLGP::Vector2f m_NetForce = LLGP::Vector2f(0.f, 0.f);//
	LLGP::Vector2f m_Velocity = LLGP::Vector2f(0.f, 0.f);
	float m_MaxSpeed = 150.f;//
	LLGP::Vector2f m_Acceleration = LLGP::Vector2f(0.f, 0.f);//
	float m_FrictionForce = 65.f;//
	float m_DragForce = 1000.f;//
	LLGP::Vector2f m_GravityForce = LLGP::Vector2f(0, 90.f);//
	float m_Mass = 1.f;//
	bool m_isGrounded = false;
	bool m_HasGravity = false;
	float m_distanceTravelled = 0;
	bool m_OpposingMovement = false;
	bool m_DoesBounce = false;

	void AddPosition(LLGP::Vector2f posToAdd);
};
