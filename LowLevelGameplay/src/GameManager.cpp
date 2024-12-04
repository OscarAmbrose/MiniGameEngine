#include <GameManager.h>
#include <Transform.h>

GameManager* GameManager::instance;

GameManager::GameManager()
{
	if (GameManager::instance != nullptr)
	{
		delete this;
		return;
	}
	GameManager::instance = this;
}

GameManager::~GameManager()
{
}

GameObject* GameManager::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	if (gameObject.get()->GetName() == "")
	{
		throw std::invalid_argument("Game Object was not given a name");
		return nullptr;
	}

	m_GameObjects.push_back(std::move(gameObject));
	return dynamic_cast<GameObject*>(m_GameObjects[m_GameObjects.size() - 1].get());
}

GameObject* GameManager::GetGameObjectByName(std::string objectName)
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->GetName() == objectName)
		{
			if (m_GameObjects[i]->m_IsGarbage == true) { std::cerr << "Warning: Be careful with getting object by name when garbage objects still exist." << std::endl; continue; }
			return m_GameObjects[i].get();
			break;
		}
	}
	return nullptr;
}

void GameManager::ClearGameObjects()
{
	m_ShouldBinObjects = true;
	for (std::shared_ptr<GameObject> obj : m_GameObjects)
	{
		if (obj->GetPersistent()) { continue; }
		obj->SetActive(false);
		obj->SetGarbage();
	}
}

void GameManager::CollectGarbage(float deltaTime)
{
	if (m_ShouldBinObjects)
	{
		m_ShouldBinObjects = false;
		m_GameObjects.erase(std::remove_if(m_GameObjects.begin(), m_GameObjects.end(), [](const std::shared_ptr<GameObject>& obj) {return obj->m_IsGarbage; }), m_GameObjects.end());
	}
}
