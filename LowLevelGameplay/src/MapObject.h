#pragma once
#include <GameObject.h>
#include <memory>
#include <Vector2.h>

class MapObject : public GameObject
{
public:
	MapObject();
	~MapObject();

	static std::shared_ptr<GameObject> NewMapObject(LLGP::Vector2f loc, LLGP::Vector2f size);
	static std::shared_ptr<GameObject> NewMapBackground(LLGP::Vector2f loc);
	static int objectCounter;
};
