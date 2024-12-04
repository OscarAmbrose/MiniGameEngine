#pragma once
#include <Object.h>
#pragma message("-------------------------Inlcuding Object in " __FILE__)
#include <iostream>

class GameObject;

struct CollisionInfo;

class Component : public Object
{
protected:
	GameObject* m_GameObject;
	bool m_Started = false;
public:
	Component(GameObject* owner);
	virtual ~Component();

	virtual void Start(float start);
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);

	GameObject* GetGameObject() { return m_GameObject; }
	virtual void OwnerActiveChange(bool newActive) {}
	
public:
	virtual void OnCollisionEnter(CollisionInfo* col) {}
	virtual void OnCollisionStay(CollisionInfo* col) {}
	virtual void OnCollisionExit(CollisionInfo* col) {}
};
