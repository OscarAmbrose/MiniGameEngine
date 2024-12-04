#pragma once
#include <UUID.h>
#include <iostream>
class Object
{
public:
	Object() : uuid() {}
	Object(const Object&) = default;

	LLGP::UUID uuid;

	inline bool operator==(const Object& rhs) { return (uint64_t)this->uuid == (uint64_t)rhs.uuid; }
	inline bool operator!=(const Object& rhs) { return !(*this == rhs); }
};
