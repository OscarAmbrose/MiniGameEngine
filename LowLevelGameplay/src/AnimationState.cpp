#pragma once
#include "AnimationState.h"
#include "AnimationManager.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <Transform.h>
#include <Event.h>

//Constructor
AnimationState::AnimationState(AnimationManager* animManager)
{
	//Sets default anim manager reference
	m_animationManager = animManager;
	
	m_GameObject = m_animationManager->GetGameObject();

	m_RigidBody = m_GameObject->GetComponent<RigidBody>();

	//We want each animation state to be false in the beginning as they are added, when we add a animation this can be set to true.
	m_Active = false;
	
	//Default animation is 0
	m_activeAnimation = 0;
}

//Deconstructor
AnimationState::~AnimationState()
{
	g_OnFixedUpdate.RemoveListener(this, std::bind(&AnimationState::FixedUpdateAnimation, this, std::placeholders::_1));
}

Animation* AnimationState::findAnimation(std::string animationName)
{

	Animation* returnAnim = nullptr;
	for (int i = 0; i < m_animations.size(); i++)
	{
		returnAnim = m_animations[i].get();
		if (returnAnim->getName() == animationName)
		{
			return returnAnim;
		}
	}
	return returnAnim;
}

void AnimationState::playAnimation(std::string animationName)
{
	Animation* animationToPlay = findAnimation(animationName);
	if (animationToPlay == nullptr)
	{
		std::cout << "AnimationState::playAnimation failed. Could not find animation." << std::endl;
		return;
	}

}

// Sets the active bool and deals with event dispatchers
void AnimationState::setActive(bool newActive)
{
	//Check if we need to do anything at all.
	//if (m_Active == newActive) { return; }
	//If we do, first resolve true.
	if (newActive == true) 
	{
		m_Active = true;
		updateRenderer(m_animations[m_activeAnimation]->getNextFrame());
		g_OnFixedUpdate.AddListener(this, std::bind(&AnimationState::FixedUpdateAnimation, this, std::placeholders::_1));
		return;
	}
	//Otherwise resolve false. Technically I could remove this else, but I think it improves readability.
	else
	{
		g_OnFixedUpdate.RemoveListener(this, std::bind(&AnimationState::FixedUpdateAnimation, this, std::placeholders::_1));
		for (int i = 0; i < m_animations.size(); i++)
		{
			m_animations[i].get()->resetAnim();
		}
	}
}

void AnimationState::speedBasedAnimation(float thisFrameDistanceTravelled)
{
	distanceTravelled += thisFrameDistanceTravelled;
	//std::cout << distanceTravelled << std::endl;
	if (distanceTravelled >= (getSpriteRenderer()->returnShape().getSize().x * ((4.5 / 32) * m_GameObject->GetTransform()->returnScale().x) ))
	{
		updateRenderer(m_animations[m_activeAnimation]->getNextFrame());
		distanceTravelled -= (getSpriteRenderer()->returnShape().getSize().x * (float)((4.5 / 32) * m_GameObject->GetTransform()->returnScale().x));
	}
}

void AnimationState::iterateAnimation()
{

}



//Just update the Renderer as a normal Tiled sprite (99% of usecase for the animator)
void AnimationState::updateRenderer(LLGP::Vector2i position)
{
	getSpriteRenderer()->setUV(position);
}

//Update animation for larger sprites (might see no use if pterodactly doesnt change
void AnimationState::updateRenderer(LLGP::Vector2i position, LLGP::Vector2i size)
{
	getSpriteRenderer()->setUV(position, size);
}

//Here will be the third sprite animation version when I animate the burning platform


//Animation update function, I figured update it better than fixed update for this.
//But it might be better to change this to fixed update, as animations shouldn't change without the physics change.
void AnimationState::FixedUpdateAnimation(float deltaTime)
{
	//Switch on which animation type is currently active
	int currentAnimationType = m_animations[m_activeAnimation].get()->returnAnimationType();

	switch (currentAnimationType) {
	case 0:
		//DefaultSizeAnimations (Input Based)
		updateRenderer(m_animations[m_activeAnimation]->getNextFrame());
		break;
	case 1:
		// (Speed Based Animations)
		// If the rigid body isnt null, update the speed based animation by distance travelled
		if (m_RigidBody != nullptr)
		{
			speedBasedAnimation(m_RigidBody->GetDistanceTravelled());
		}
		//If rigid body is null do this instead
		else
		{
			return;
		}
		//If stop moving go to default pose
		if (m_RigidBody->GetVelocity().GetMagnitude() >= -0.5 && m_RigidBody->GetVelocity().GetMagnitude() <= 0.5)
		{
			m_animations[m_activeAnimation].get()->resetAnim();
			updateRenderer(m_animations[m_activeAnimation].get()->getNextFrame());
		}
		break;
	case 2:
		//LargeStyleAnimations (TimeBased)
		break;
	default:
		//CodeInfo
		break;
	}
}

void AnimationState::setActiveAnimation(std::string animationName)
{
	Animation* animationToActivate = findAnimation(animationName);
	for (int i = 0; i < m_animations.size(); i++)
	{
		if (m_animations[i].get() == animationToActivate)
		{
			if (m_activeAnimation == i)
			{
				return;
			}
			m_animations[m_activeAnimation].get()->resetAnim();
			setActiveAnimation(i);
			updateRenderer(m_animations[m_activeAnimation]->getNextFrame());
		}
	}
}

void AnimationState::setActiveAnimation(int animationName)
{
	m_activeAnimation = animationName;
}

inline SpriteRenderer* AnimationState::getSpriteRenderer()
{
	return m_animationManager->returnSpriteRenderer();
}
