#pragma once
#include <memory>
#include <vector>
#include <Vector2.h>
#include <iostream>
#include <SpriteRenderer.h>
#include <Animation.h>
#include <cstdarg>

class AnimationManager;
class RigidBody;

template<typename T>
concept isAnimation = std::derived_from<T, Animation> == true;

class AnimationState
{
public:
	AnimationState(AnimationManager* animManager);

	~AnimationState();

	//I originially templated this function but I realised I can keep all of my animations
	//the same so a regular function will work.
	Animation* AddAnimation(std::string name, int animType, int animLength, ...)
	{
		std::unique_ptr<Animation> newAnim = std::make_unique<Animation>();

		newAnim.get()->setName(name);
		newAnim.get()->setAnimationType(animType);

		va_list args;
		va_start(args, animLength);
		newAnim.get()->addAnimation(animLength, args);
		va_end(args);

		m_animations.push_back(std::move(newAnim));
		return dynamic_cast<Animation*>(m_animations[m_animations.size() - 1].get());
	}

	//Possibly add a remove animation function here, although I'm not sure I want/need to yet.
	//Each Animation state will probably derive from AnimationState, not be a single class for
	//ease of development later on.

	Animation* findAnimation(std::string animationName);

	void playAnimation(std::string animationName);

	void setActive(bool newActive);

	/// <summary>
	/// This function is intended as one of the main animation types for my "engine"
	/// All bird based characters will use this form of animation for walking
	/// </summary>
	/// <param name="Speed"></param>
	void speedBasedAnimation(float thisFrameDistanceTravelled);

	void iterateAnimation();

	void updateRenderer(LLGP::Vector2i position);

	void updateRenderer(LLGP::Vector2i position, LLGP::Vector2i size);

	//void updateRenderer(LLGP::Vector2i position, LLGP::Vector2i size, LLGP::Vector2i spriteOrigin);

	//Global events to be bound to enable/disable the animation state.
	void FixedUpdateAnimation(float deltaTime);

	inline void setName(std::string newName) { m_animationStateName = newName; }
	inline std::string getName() { return m_animationStateName; }

	void setActiveAnimation(std::string animationName);
	void setActiveAnimation(int animationName);

protected:
	//Designed to make this class more readable.
	inline SpriteRenderer* getSpriteRenderer();

	GameObject* m_GameObject;

	RigidBody* m_RigidBody;

	//To check whether this animation state is Active.
	bool m_Active;

	//List of animation unique ptrs.
	std::vector<std::unique_ptr<Animation>> m_animations;
	//The item in the list, which is the active animation.
	int m_activeAnimation;

	//The parent animation manager.
	AnimationManager* m_animationManager;

	float currentTime = 0;
	float distanceTravelled = 0;

	std::string m_animationStateName;
};
