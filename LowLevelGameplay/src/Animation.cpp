#include "Animation.h"
#include <cstdarg>

#pragma region Constructors & Destructors

Animation::Animation() : Object()
{
	animationPositions = std::make_shared<std::vector<LLGP::Vector2i>>();
	currentFrame = -1;
	animType = 0;
}

Animation::Animation(std::vector<LLGP::Vector2i> animationPos)
{
	animationPositions = std::make_shared<std::vector<LLGP::Vector2i>>();
	for (int i = 0; i < animationPos.size(); i++)
	{
		addAnimationFrame(animationPos[i]);
	}
	currentFrame = -1;
	animType = 0;
}

Animation::Animation(std::vector<LLGP::Vector2i> animationPos, int animationType)
{
	animationPositions = std::make_shared<std::vector<LLGP::Vector2i>>();
	for (int i = 0; i < animationPos.size(); i++)
	{
		addAnimationFrame(animationPos[i]);
	}
	currentFrame = -1;
	animType = 0;
}

Animation::~Animation()
{

}

#pragma endregion

//Used to retrieve and iterate the next frame of the animation
LLGP::Vector2i Animation::getNextFrame()
{
	//Get a pointer to the vector of vectorIs
	std::vector<LLGP::Vector2i>* tempVector = animationPositions.get();

	//Error checking to see if an animation has been loaded
	if (tempVector == nullptr || tempVector->size() == 0)
	{
		std::cout << "Animation not loaded" << std::endl;
		return LLGP::Vector2i(0, 0);
	}

	//First check we havent reached the end of the list
	if (currentFrame < (tempVector->size() - 1))
	{
		currentFrame++;
	}
	//If we did, reset it to Zero.
	else
	{
		currentFrame = 0;
	}

	//std::cout << (*tempVector)[currentFrame].x << std::endl;
	
	return (*tempVector)[currentFrame];
}

// I decided to use variadic arguments to create my animations.
// This was to make it easier to create the animations.
void Animation::addAnimation(int numArgs, va_list args)
{
	//Create my variadic arg list
	//Loop through all of my arguments
	for (int i = 0; i < numArgs; i++)
	{
		//Get the next Vector2i and push it back into my list.
		LLGP::Vector2i nextPos = va_arg(args, LLGP::Vector2i);
		animationPositions->push_back(nextPos);
	}
	//end because its necesarry to clean up data?
}

//Adds a single frame to the animation
inline void Animation::addAnimationFrame(LLGP::Vector2i framePosition)
{
	animationPositions.get()->push_back(framePosition);
}
