#include <Physics.h>
#pragma message("-------------------------Inlcuding Physics in " __FILE__)
#include <RigidBody.h>
#pragma message("-------------------------Inlcuding RigidBody in " __FILE__)
#include <GameObject.h>
#pragma message("-------------------------Inlcuding GameObject in " __FILE__)
#include <BoxCollider.h>
#pragma message("-------------------------Inlcuding BoxCollider in " __FILE__)
#include <Transform.h>

std::vector<CollisionInfo*> Physics::_collisions;
std::vector<CollisionInfo*> Physics::_reversedCollisions;
std::vector<CollisionInfo*> Physics::_oldCollisions;

std::vector<Collider*> Physics::_colliders;
std::vector<RigidBody*> Physics::_rigidBodies;

bool CollisionInfo::operator==(CollisionInfo* other)
{
	//If objects are both equal (in either direction)
	if ((Object)*(this->collider) == (Object)*(other->collider) && (Object) * (this->otherCollider) == (Object) * (other->otherCollider)
		|| (Object) * (this->otherCollider) == (Object) * (other->collider) && (Object) * (this->otherCollider) == (Object) * (other->collider))
	{
		return true;
	}
	else
	{
		return false;
	}
	//return (*(this->collider) == *(other->collider) && *(this->otherCollider) == *(other->otherCollider)) || (*(this->collider) == *(other->otherCollider) && *(this->otherCollider) == *(other->collider));
}

void Physics::RegisterColldier(Collider* col)
{
	if (std::find_if(_colliders.begin(), _colliders.end(), [col](Collider* check) { return *col == *check; }) == _colliders.end())
	{
		_colliders.push_back(col);
	}
}

void Physics::DeregisterColldier(Collider* col)
{
	std::erase_if(_colliders, [col](Collider* check) { return *col == *check; });
}

void Physics::RegisterRigidBody(RigidBody* rb)
{
	if (std::find_if(_rigidBodies.begin(), _rigidBodies.end(), [rb](RigidBody* check) { return *rb == *check; }) == _rigidBodies.end())
	{
		_rigidBodies.push_back(rb);
	}
}

void Physics::DeregisterRigidBody(RigidBody* rb)
{
	std::erase_if(_rigidBodies, [rb](RigidBody* check) { return *rb == *check; });
}

void Physics::CollectCollisions()
{
	//For each game object with a rigidbody
	for (RigidBody* rb : _rigidBodies)
	{
		//get all of the rigid body game objects with a colldier
		std::vector<Collider*> rbCols = rb->GetGameObject()->GetComponents<Collider>();
		//For each rigid body in my rigid body list, check against all other colliders
		for (Collider* rbCol : rbCols)
		{
			//Check against all world colliders
			for (Collider* worldCol : _colliders)
			{
				//If I AM that collider, continue.
				if (*rbCol == *worldCol || *rbCol->GetGameObject() == *worldCol->GetGameObject()) { continue; }

				//or if the either object isnt active, continue.
				if (!rbCol->GetGameObject()->GetActive() || !worldCol->GetGameObject()->GetActive()) { continue; }

				//If the collision info already exists, continue. Or, if the collision masks don't match, continue. 
				{

					CollisionInfo* test = new CollisionInfo(); test->collider = rbCol; test->otherCollider = worldCol;

					//I added bitwise collision masking :)
					if (((test->collider->GetCollisionMask() & test->otherCollider->GetCollisionLayer())) == 0 && ((test->otherCollider->GetCollisionMask() & test->collider->GetCollisionLayer())) == 0)
					{
						//ALMOST LET A MEMORY LEAK IN 23/05/24!
						delete test;
						continue;
					}
					//End of bitwise collision masking :(	

					if (std::find_if(_collisions.begin(), _collisions.end(), [&test](CollisionInfo* check) {return *test == check; }) != _collisions.end())
					{
						delete test;
						continue;
					}
					//Found the memory leak, fix is here
					delete test;
				}
				//If collision isnt the same, throw it back
				if (CollisionInfo* collision = rbCol->IsCollidingWith(worldCol))
				{
					_collisions.push_back(collision);
				}
			}
		}
	}
}

//Dispatch everything
void Physics::DispatchCollisions()
{
	//Go backwards down the list
	for (int newIndex = _collisions.size() - 1; newIndex >= 0; newIndex--)
	{
		bool newCollision = true;
		//If the collision exists in the old list, return true and call on collision stay etc

		for (int oldIndex = (_oldCollisions.size() - 1); oldIndex >= 0; oldIndex--)
		{
			//100000 years of debugging lies here
			if ((*_collisions[newIndex] != _oldCollisions[oldIndex])) { continue; }
			//Collision Stay
			_collisions[newIndex]->collider->GetGameObject()->onCollisionStay(_collisions[newIndex]);
			if (_collisions[newIndex]->otherIsRB) { _collisions[newIndex]->otherCollider->GetGameObject()->onCollisionStay(ReverseCollision(_collisions[newIndex])); }
			delete _oldCollisions[oldIndex];
			_oldCollisions.erase(std::next(_oldCollisions.begin(), oldIndex));
			newCollision = false;
			break;
		}
		
		
		if (newCollision)
		{
			//Collision Enter
			_collisions[newIndex]->collider->GetGameObject()->onCollisionEnter(_collisions[newIndex]);
			if (_collisions[newIndex]->otherIsRB) { _collisions[newIndex]->otherCollider->GetGameObject()->onCollisionEnter(ReverseCollision(_collisions[newIndex])); }
		}

	}

	for (int i = (_oldCollisions.size() - 1); i >= 0; i--)
	{
		//Collision Exit
		_oldCollisions[i]->collider->GetGameObject()->onCollisionExit(_oldCollisions[i]);
		if (_oldCollisions[i]->otherIsRB)
		{
			_oldCollisions[i]->otherCollider->GetGameObject()->onCollisionExit(ReverseCollision(_oldCollisions[i]));
		}
		delete _oldCollisions[i];
	}
	
	
	_oldCollisions.clear();
	_oldCollisions = _collisions;
	_collisions.clear();
	for (CollisionInfo* C : _reversedCollisions)
	{
		delete C;
	}
	_reversedCollisions.clear();
}

CollisionInfo* Physics::ReverseCollision(CollisionInfo* in)
{
	CollisionInfo* reverse = new CollisionInfo();
	reverse->collider = in->otherCollider;
	reverse->otherCollider = in->collider;

	if (in->collider->GetGameObject()->GetComponent<RigidBody>() != nullptr)
	{
		reverse->otherIsRB = true;
	}
	reverse->Normal = -in->Normal;
	//_reversedCollisions.push_back(std::move(reverse));
	_reversedCollisions.push_back(reverse);
	//delete reverse;
	//return _reversedCollisions.back();
	return reverse;
}

float Physics::CalculateImpulse(RigidBody* rb1, RigidBody* rb2)
{
	return 0.f;
}

float Physics::CalculateImpulse(RigidBody* rb1)
{
	return 0.f;
}

CollisionInfo* Physics::Collision_AABBAABB(BoxCollider* lhs, BoxCollider* rhs)
{
	if(!lhs->GetIsTrigger()) {lhs->SetBoxPosition(lhs->GetGameObject()->GetTransform()->ReturnPosition());}
	if(!rhs->GetIsTrigger()) { rhs->SetBoxPosition(rhs->GetGameObject()->GetTransform()->ReturnPosition());}

	if ((lhs->GetBoxPosition().x + lhs->GetBoxHalfExtents().x < rhs->GetBoxPosition().x - rhs->GetBoxHalfExtents().x) ||
		(lhs->GetBoxPosition().y + lhs->GetBoxHalfExtents().y < rhs->GetBoxPosition().y - rhs->GetBoxHalfExtents().y) ||
		(lhs->GetBoxPosition().x - lhs->GetBoxHalfExtents().x > rhs->GetBoxPosition().x + rhs->GetBoxHalfExtents().x) || 
		(lhs->GetBoxPosition().y - lhs->GetBoxHalfExtents().y > rhs->GetBoxPosition().y + rhs->GetBoxHalfExtents().y))
	{
		return nullptr;
	}

	CollisionInfo* newCollisionInfo = new CollisionInfo();

	float penDepthX = 0;
	float penDepthY = 0;

	LLGP::Vector2f collisionNormal = LLGP::Vector2f(0, 0);

#pragma region		GetPenDepth
	//GetPenDepthX
	if (lhs->GetBoxPosition().x < rhs->GetBoxPosition().x)
	{
		penDepthX = (lhs->GetBoxPosition().x + lhs->GetBoxHalfExtents().x) - (rhs->GetBoxPosition().x - rhs->GetBoxHalfExtents().x);
		collisionNormal = LLGP::Vector2f(-1, 0);
	}
	else
	{
		penDepthX = (rhs->GetBoxPosition().x + rhs->GetBoxHalfExtents().x) - (lhs->GetBoxPosition().x - lhs->GetBoxHalfExtents().x);
		collisionNormal = LLGP::Vector2f(1, 0);
	}

	//GetPenDepthY
	if (lhs->GetBoxPosition().y > rhs->GetBoxPosition().y)
	{
		penDepthY = (rhs->GetBoxPosition().y + rhs->GetBoxHalfExtents().y) - (lhs->GetBoxPosition().y - lhs->GetBoxHalfExtents().y);
		collisionNormal = LLGP::Vector2f(collisionNormal.x, 1);
	}
	else
	{
		penDepthY = (lhs->GetBoxPosition().y + lhs->GetBoxHalfExtents().y) - (rhs->GetBoxPosition().y - rhs->GetBoxHalfExtents().y);
		collisionNormal = LLGP::Vector2f(collisionNormal.x, -1);
	}

	if (penDepthX < penDepthY)
	{
		newCollisionInfo->Overlap = penDepthX;
		collisionNormal = LLGP::Vector2f(collisionNormal.x, 0);
	}
	else
	{
		newCollisionInfo->Overlap = penDepthY;
		collisionNormal = LLGP::Vector2f(0, collisionNormal.y);
	}
#pragma endregion

	newCollisionInfo->collider = lhs; 
	newCollisionInfo->otherCollider = rhs;

	newCollisionInfo->Normal = collisionNormal;

	float calculatedImpulse;

	RigidBody* rbTest;
	RigidBody* col1RigidBody = lhs->GetGameObject()->GetComponent<RigidBody>();

	if (rbTest = (rhs->GetGameObject()->GetComponent<RigidBody>()))
	{
		newCollisionInfo->otherIsRB = true;
		calculatedImpulse = CalculateImpulse(col1RigidBody, rbTest);
	}
	else
	{
		CalculateImpulse(col1RigidBody);
	}

	//std::cout << "I COLLIDED" << std::endl;
	return newCollisionInfo;
}

///Bit Operators (for my own memory), I've managed to get my heap memory down tons using these (20kb->5kb)

// u_int8 a // 00000111
// u_int8 b // 00000001

// a = a ^ b;  00000110
// b = b ^ a;  00000111
// a = a ^ b;  00000001


//Im have implemented this! This is for my notes.
// u_int8 colLayer     //00000001
// u_int8 detectLayer  //11111001

// (colLayer & detectLayer) != 0 //

// << (bit shift but wraps)
// <<< (bit shift but doesnt wrap)

// 10000001
// 00000011 << 
