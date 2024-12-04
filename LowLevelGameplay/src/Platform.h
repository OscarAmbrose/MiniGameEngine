#pragma once
#include <GameObject.h>
#include <Vector2.h>

class Platform : public GameObject
{
public:
	Platform();
	~Platform();

	static bool randomBool();

	static std::shared_ptr<Platform> CreatePlatformType1(LLGP::Vector2f platformLocation);
	static std::shared_ptr<Platform> CreateCeiling();

	Platform* SetPlatformInformation(LLGP::Vector2i platformStart, LLGP::Vector2i platformSize);

	static void AddDebugBox(Platform* platform);

protected:
	LLGP::Vector2f m_PlatformSize = LLGP::Vector2f(0,0);
};
