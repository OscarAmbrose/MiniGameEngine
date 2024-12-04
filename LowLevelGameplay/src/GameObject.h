#pragma once
#include <Object.h>
#include <string>
#include <vector>
#include <memory>
#include <isComponent.h>
//#include <Transform.h>
#include <iostream>
#include <Event.h>

struct CollisionInfo;

class Transform2D;

class GameObject : public Object
{
public:
	GameObject();
	GameObject(const GameObject&) = default;

	~GameObject() {}

	LLGP::Event<CollisionInfo*> onCollisionEnter;
	LLGP::Event<CollisionInfo*> onCollisionStay;
	LLGP::Event<CollisionInfo*> onCollisionExit;
	
	GameObject* SetName(std::string newName) { m_Name = newName; return this; }
	inline std::string GetName() { return m_Name; }

	virtual void SetActive(bool newActive) { m_Active = newActive; }
	inline bool GetActive() { return m_Active; }

	inline void SetTag(std::string newTag) { m_Tag = newTag; }
	inline bool CompareTag(std::string comp) { return m_Tag == comp; }

	Transform2D* GetTransform() { return transform.get(); }

	void SetGarbage() 
	{ 
		if (m_IsPersistent) { return; } 
		m_IsGarbage = true;
	}

	GameObject* SetPersistent(bool newPersistent) { m_IsPersistent = newPersistent; return this; }
	bool GetPersistent() const { return m_IsPersistent; }

#pragma region ComponentManagement
	template<class T> requires isComponent<T>
	T* GetComponent()
	{
		T* returnComp = nullptr;

		for (int i = 0; i < m_Components.size(); i++)
		{
			returnComp = dynamic_cast<T*>(m_Components[i].get());
			if (returnComp != nullptr)
			{
				return returnComp;
			}
		}

		return nullptr;
	};

	template<class T> requires isComponent<T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> returnComps;

		for (int i = 0; i < m_Components.size(); i++)
		{
			if (T* comp = dynamic_cast<T*>(m_Components[i].get()))
			{
				returnComps.push_back(comp);
			}
		}

		return returnComps;
	};

	template<class T> requires isComponent<T> 
	T* AddComponent() 
	{
		m_Components.push_back(std::make_unique<T>(this));
		return dynamic_cast<T*>(m_Components[m_Components.size()-1].get());
	};
	
	template<class T> requires isComponent<T> 
	bool RemoveComponent()
	{
		bool returnBool = false;
		T* returnComp = nullptr;

		for (int i = m_Components.size()-1; i >= 0; i--)
		{
			returnComp = dynamic_cast<T*>(m_Components[i].get());
			if (returnComp != nullptr)
			{
				returnBool = true;
				m_Components.erase(m_Components.begin() + i);
				break;
			}
			returnBool = false;
		}
		return returnBool;
	};

#pragma endregion

protected:
	bool m_Active = false;
	std::string m_Name;
	std::string m_Tag;
	std::vector<std::unique_ptr<Component>> m_Components;
	bool m_IsPersistent = false;
private:

public:
	std::unique_ptr<Transform2D> transform = std::make_unique<Transform2D>();
	bool m_IsGarbage = false;
};
