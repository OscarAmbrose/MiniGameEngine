#pragma once
#include <Vector2.h>
#include <vector>
#include "Collider.h"
#include "BoxCollider.h"

class RigidBody;

struct CollisionInfo
{
	CollisionInfo() {};

	Collider* collider = nullptr;
	Collider* otherCollider = nullptr;
	bool otherIsRB = false;
	LLGP::Vector2f Location = LLGP::Vector2f(0.f, 0.f);
	LLGP::Vector2f Normal = LLGP::Vector2f(0.f, 0.f);
	float Overlap = 0.f;
	float Impulse = 0.f;
	
	bool operator==(CollisionInfo* other);
	bool operator!=(CollisionInfo* other) { return !(*this == other); };

};

class Physics
{
public:
	Physics() = delete;
	static void RegisterColldier(Collider* col);
	static void DeregisterColldier(Collider* col);
	static void RegisterRigidBody(RigidBody* rb);
	static void DeregisterRigidBody(RigidBody* rb);
	static void CollectCollisions();
	static void DispatchCollisions();
	static CollisionInfo* ReverseCollision(CollisionInfo* in);
	static float CalculateImpulse(RigidBody* rb1, RigidBody* rb2);
	static float CalculateImpulse(RigidBody* rb1);

	static CollisionInfo* Collision_AABBAABB(BoxCollider* lhs, BoxCollider* rhs);

private:
	static std::vector<RigidBody*> _rigidBodies;
	static std::vector<Collider*> _colliders;
	static std::vector<CollisionInfo*> _collisions;
	static std::vector<CollisionInfo*> _reversedCollisions;
	static std::vector<CollisionInfo*> _oldCollisions;
};
