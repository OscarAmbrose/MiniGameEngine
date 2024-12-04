#pragma once
#include <Component.h>
//#include <arithmetic.h>
#include <InputAsset.h>
#include <vector>
#include <GlobalEvents.h>

template <class T>
concept isInputAsset = std::derived_from<T, InputAssetBase> == true;

class InputManager : public Component
{
public:
	InputManager(GameObject* owner) : Component(owner)
	{
		g_OnPollInputs.AddListener(this, std::bind(&InputManager::PollInput, this, std::placeholders::_1));
	}

	~InputManager() 
	{
		g_OnPollInputs.RemoveListener(this, std::bind(&InputManager::PollInput, this, std::placeholders::_1));

	}

	virtual void PollInput(sf::Event i)
	{
	}

	template<class T>
	T* addEvent(std::string eventName)
	{
		std::unique_ptr<InputAssetBase> newInputAsset = std::make_unique<T>();
		newInputAsset.get()->setName(eventName);
		m_inputAssets.push_back(std::move(newInputAsset));
		return dynamic_cast<T*>(m_inputAssets[m_inputAssets.size() - 1].get());
	}

	template<class T> requires isInputAsset<T>
	bool RemoveInputAsset(std::string eventName)
	{
		bool returnBool = false;
		T* returnInputAsset = nullptr;
		for (int i = 0; i < m_inputAssets.size(); i++)
		{
			if (m_inputAssets[i].get()->getName() == eventName)
			{
				returnBool = true;
				m_inputAssets.erase(m_inputAssets.begin() + i);
				break;
			}
			returnBool = false;
		}
		return returnBool;
	}

	template<class T>
	T* GetInputAction(std::string eventName)
	{
		T* returnInputAction = nullptr;

		for (int i = 0; i < m_inputAssets.size(); i++)
		{
			if (eventName == m_inputAssets[i].get()->getName())
			{
				returnInputAction = dynamic_cast<T*>(m_inputAssets[i].get());
				break;
			}
		}
		return returnInputAction;
	}

	template<typename T>
	LLGP::Event<T>* getEvent(std::string eventName)
	{
		LLGP::Event<T>* eventPointer = nullptr;
		return GetInputAction<InputAsset<T>>(eventName)->getEvent<T>();
	}

	template<typename T>
	void invokeEvent(std::string eventName, T value)
	{
		GetInputAction<InputAsset<T>>(eventName)->CallEvent(value);
	}

protected:
	std::vector<std::unique_ptr<InputAssetBase>> m_inputAssets;
};
