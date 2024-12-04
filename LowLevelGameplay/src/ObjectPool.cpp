#include "ObjectPool.h"
#include <GameObject.h>
#include <Projectile.h>
#include <Transform.h>

std::vector<Projectile*> ObjectPooler::_UnusedObjects;
std::vector<Projectile*> ObjectPooler::_UsedObjects;
size_t const _DefaultPoolSize = 100;

void ObjectPooler::InitialisePool()
{
	_UnusedObjects.reserve(_DefaultPoolSize);
	_UsedObjects.reserve(_DefaultPoolSize);
	for (int i = 0; i < _DefaultPoolSize; i++)
	{
		_UnusedObjects.emplace_back(std::move(new Projectile()));
	}
}

void ObjectPooler::DeactivateObject(Projectile* objectToDeactivate)
{
	for (int i = 0; i < _UsedObjects.size(); i++)
	{
		if (_UsedObjects[i] == objectToDeactivate)
		{
			_UnusedObjects.push_back(std::move(_UsedObjects[i]));
			_UsedObjects.erase(_UsedObjects.begin() + i);
			//std::cout << "Unused Objects size: " << _UnusedObjects.size() << "\n";
			//std::cout << "Used Objects size: " << _UsedObjects.size() << "\n";
			return;
		}
	}
}

size_t ObjectPooler::GetRemainingObjects()
{
	return _UnusedObjects.size();
}

Projectile* ObjectPooler::FindRemainingObject()
{
	if (_UnusedObjects.size() == 0)
	{
		_UnusedObjects.push_back(std::move(new Projectile()));
	}
	_UsedObjects.push_back(std::move(_UnusedObjects.back()));
	_UnusedObjects.erase(_UnusedObjects.end() - 1);

	//std::cout << "unused objects size (WhenGettingObject): " << _UnusedObjects.size() << std::endl;
	//std::cout << "Used objects size (WhenGettingObject): " << _UsedObjects.size() << std::endl;

	return _UsedObjects.back();
}

Projectile* ObjectPooler::AddNewObject()
{
	return nullptr;
}

void ObjectPooler::DebugDisableAllPoolObjects()
{
	for (int i = _UsedObjects.size() - 1; i >= 0; i--)
	{
		_UsedObjects[i]->DisableProjectile();
	}
}
