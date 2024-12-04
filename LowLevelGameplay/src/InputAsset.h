#pragma once
#include <Object.h>
#include <arithmetic.h>
#include<Event.h>

class InputAssetBase : public Object 
{
public:
	InputAssetBase() : Object() {};
	virtual ~InputAssetBase() {};

	template<typename U> requires arithmetic<U>
	LLGP::Event<U>* getEvent() { return nullptr; }

	void setName(std::string newName) { eventName = newName; }
	std::string getName() { return eventName; }

protected:
	std::string eventName;
};

template<typename T>
class InputAsset : public InputAssetBase
{
public:
	InputAsset() { eventName = ""; }
	InputAsset(std::string name) { eventName = name; }

	~InputAsset() {};

	//T* getReturnType(){ return class T; }

	void CallEvent(T output)
	{
		inputEvent.get()->Invoke(output);
	}

	template<typename T>
	LLGP::Event<T>* getEvent()
	{ 
		return inputEvent.get();
	}

protected:
	std::unique_ptr<LLGP::Event<T>> inputEvent = std::make_unique<LLGP::Event<T>>();
};;


