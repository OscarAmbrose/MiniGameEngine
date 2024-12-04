#pragma once
#include <vector>
#include <memory>
#include <GameObject.h>
#include <isComponent.h>
#include <isGameObject.h>


class GameManager
{
public:
	/// <summary>
	/// Singleton pattern game manager.
	/// </summary>

	GameManager();

	~GameManager();

	/// <summary>
	/// Returns all components visible to the game manager of type T. 
	/// </summary>
	/// <typeparam name="T">Component Type</typeparam>
	/// <returns></returns>
	template<class T> requires isComponent<T>
	std::vector<T*> getAllComponentsOfType()
	{
		std::vector<T*> gatheredList;

		for (int i = 0; i < m_GameObjects.size(); i++)
		{
			if (auto gameObject = m_GameObjects[i]) // Check if gameObject is not null
			{
				T* discoveredComponent = gameObject->GetComponent<T>();
				if (discoveredComponent != nullptr)
				{
					gatheredList.push_back(discoveredComponent);
				}
			}
		}
		return gatheredList;
	}

	/// <summary>
	/// Used to add a component to the game object. 
	/// <para>Use this when you want the game manager to remove objects when loading a new level.</para>
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T> requires isGameObject<T>
	T* AddGameObject()
	{
		m_GameObjects.push_back(std::make_shared<T>());
		return static_cast<T*>(m_GameObjects[m_GameObjects.size() - 1].get());
	}	
	

	template<class T> requires isGameObject<T>
	std::vector<T*> getAllObjectsOfType()
	{
		std::vector<T*> gatheredObjects;
		for (std::shared_ptr<GameObject> object : m_GameObjects)
		{
			if (T* foundObject = dynamic_cast<T*>(object.get()))
			{
				gatheredObjects.push_back(foundObject);
			}
		}
		return gatheredObjects;
	}

	/// <summary>
	/// Used to add a game object that needs extra initialisation, you can create it where you need and add it to game managers vector using this function.
	/// <para>The game object requires to be named using the "GetName()" function, otherwise this function will return an error.</para>
	/// See Also: <seealso cref="T* GameManager::AddGameObject"/>
	/// </summary>
	/// <param name="gameObject">: the shared_ptr GameObject that you are adding. This can be any class that inherits from game object.</param>
	/// <returns></returns>
	GameObject* AddGameObject(std::shared_ptr<GameObject> gameObject);

	/// <summary>
	/// Returns an unsafe pointer to the game object with the matching name.
	/// <para> . </para>
	/// <para>If no GameObject is found, returns nullptr.</para>
	/// </summary>
	/// <param name="objectName">A string value equal to search for the game object.</param>
	/// <returns></returns>
	GameObject* GetGameObjectByName(std::string objectTag);

	/// <summary>
	/// Starts the game.
	/// </summary>
	virtual void Start() {}

	void ClearGameObjects();

	virtual void CollectGarbage(float deltaTime);
	void SetShouldBin(bool newBin) { m_ShouldBinObjects = newBin; }

protected:
	/// <summary>
	/// A list of all game objects visible to the Game Manager.
	/// </summary>
	std::vector<std::shared_ptr<GameObject>> m_GameObjects;

public:
	static GameManager* instance;
private:
	bool m_ShouldBinObjects = false;
};
