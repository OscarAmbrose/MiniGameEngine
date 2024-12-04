#include "AnimationManager.h"
#include "GameObject.h"
#include <Transform.h>

bool AnimationManager::findSpriteRenderer()
{
	spriteRenderer = nullptr;
	if (spriteRenderer = m_GameObject->GetComponent<SpriteRenderer>())
	{
		return true;
	}
	else
	{
		return false;
	}
}

SpriteRenderer* AnimationManager::returnSpriteRenderer()
{
	return spriteRenderer;
}

void AnimationManager::FixedUpdate(float deltaTime)
{
	
}
