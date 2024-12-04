#include "Collider.h"
#include "GameObject.h"
#include "Physics.h"
#include "BoxCollider.h"
#include <Transform.h>

Collider::Collider(GameObject* owner) : Component(owner)
{
	Physics::RegisterColldier(this);
}

Collider::~Collider()
{
	Physics::DeregisterColldier(this);
}

