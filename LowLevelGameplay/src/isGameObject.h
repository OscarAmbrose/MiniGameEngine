#pragma once
#include <GameObject.h>

template<typename T>
concept isGameObject = std::derived_from<T, GameObject> == true;
