#pragma once
#include <SpriteRenderer.h>
#include <stdexcept>
#include <vector>
#include <memory>
#include "AnimationState.h"

class AnimationManager : public Component
{
public:
	AnimationManager(GameObject* owner) : Component(owner)
	{
		if (findSpriteRenderer() == false)
		{
			throw std::invalid_argument("Game Object contains no Sprite Renderer.");
		}
		std::vector<LLGP::Vector2i> animArgu;
		for (int i = 0; i < 4; i++)
		{
			animArgu.push_back(LLGP::Vector2i(i, 0));
		}
	};

	template<class T>
	T* addAnimationState(std::string animStateName)
	{
		std::unique_ptr<AnimationState> newAnimState = std::make_unique<T>(this);
		newAnimState.get()->setName(animStateName);
		m_animationStates.push_back(std::move(newAnimState));
		return dynamic_cast<T*>(m_animationStates[m_animationStates.size() - 1].get());
	}

	template<class T>
	T* GetAnimationState(std::string animStateName)
	{
		T* returnAnimState = nullptr;

		for (int i = 0; i < m_animationStates.size(); i++)
		{
			if (animStateName == m_animationStates[i].get()->getName())
			{
				returnAnimState = dynamic_cast<T*>(m_animationStates[i].get());
				break;
			}
		}
		return returnAnimState;
	}

	template<class T>
	bool setActiveAnimationState(std::string newActiveStateName)
	{
		if (newActiveStateName == activeAnimationState)
		{
			return true;
		}

		AnimationState* newActiveState = nullptr;
		if ((newActiveState = GetAnimationState<T>(newActiveStateName)) == nullptr)
		{
			std::cout << "Animation State '" << newActiveState << "' not found." << std::endl;
			return false;
		}
		if (activeAnimationState.empty())
		{
			GetAnimationState<T>(newActiveStateName)->setActive(true);
			activeAnimationState = newActiveStateName;
			return true;
		}
		else
		{
			GetAnimationState<AnimationState>(activeAnimationState)->setActive(false);
			GetAnimationState<T>(newActiveStateName)->setActive(true);
			activeAnimationState = newActiveStateName;
			return true;
		}
		return false;
	}

	inline std::string ReturnActiveAnimationState() { return activeAnimationState; }

	bool findSpriteRenderer();

	SpriteRenderer* returnSpriteRenderer();

	void FixedUpdate(float deltaTime);

protected:
	SpriteRenderer* spriteRenderer;
	
	std::vector<std::unique_ptr<AnimationState>> m_animationStates;
	std::string activeAnimationState;

	Animation* testAnim;
};
