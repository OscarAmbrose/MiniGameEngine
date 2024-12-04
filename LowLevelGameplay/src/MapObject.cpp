#include "MapObject.h"
#include <Transform.h>
#include <BoxCollider.h>
#include <DebugBox.h>
#include <SpriteRenderer.h>
#include <GlobalTexture.h>

int MapObject::objectCounter = 0;

MapObject::MapObject()
{
}

MapObject::~MapObject()
{
}

std::shared_ptr<GameObject> MapObject::NewMapObject(LLGP::Vector2f loc, LLGP::Vector2f size)
{
	MapObject::objectCounter++;
	loc.x += (size.x / 2);
	loc.y += (size.y / 2);
	std::shared_ptr<MapObject> newMapObjPtr = std::make_unique<MapObject>();
	newMapObjPtr->AddComponent<BoxCollider>()->SetUpCollider(size, LLGP::Vector2f(0, 0))->SetCollisionLayer(0b00000001);
	newMapObjPtr->SetName("MapObject"+std::to_string(MapObject::objectCounter));
	newMapObjPtr->GetTransform()->SetPosition(loc);
	newMapObjPtr->SetPersistent(true);
	//newMapObjPtr->AddComponent<DebugBox>()->SetUpDebugBox();
	newMapObjPtr->SetActive(true);
	return newMapObjPtr;
}

std::shared_ptr<GameObject> MapObject::NewMapBackground(LLGP::Vector2f loc)
{
	std::shared_ptr<MapObject> newMapObjPtr = std::make_unique<MapObject>();
	newMapObjPtr->AddComponent<SpriteRenderer>()->SetRenderTexture(mapRenderTexture)->setUV(LLGP::Vector2i(0,0), LLGP::Vector2i(1920, 1080))->setRenderLayer(0);
	newMapObjPtr->SetName("MapBackground")->SetActive(true);
	newMapObjPtr->GetTransform()->SetPosition(loc);
	newMapObjPtr->SetPersistent(true);
	return newMapObjPtr;
}
