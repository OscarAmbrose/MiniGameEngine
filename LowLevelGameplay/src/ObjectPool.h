#pragma once
#include <Object.h>
#include <vector>
#include <isGameObject.h>
#include <memory>
#include <Projectile.h>

//TO DO: Convert to templated object pooler class
class ObjectPooler
{
public:
	ObjectPooler() = delete;

	static void InitialisePool();

	static void DebugDisableAllPoolObjects();

	static void DeactivateObject(Projectile* objectToDeactivate);
	static size_t GetRemainingObjects();
	static Projectile* FindRemainingObject();
	static Projectile* AddNewObject();

private:
	static std::vector<Projectile*> _UnusedObjects;
	static std::vector<Projectile*> _UsedObjects;

	static size_t const _DefaultPoolSize = 150;
};
