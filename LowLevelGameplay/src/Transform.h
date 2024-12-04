#pragma once
#include <Vector2.h>
#include <Event.h>

class GameObject;

class Transform2D
{
public:
	Transform2D()
	{
		m_Rotation = 0.0f;
		m_Position = LLGP::Vector2<float>(750, 500);
		m_Scale = LLGP::Vector2<float>(1, 1);
	};

	~Transform2D() { OnPositionUpdate.Empty(); }

	void SetPosition(LLGP::Vector2<float> newPosition) { m_Position = newPosition; OnPositionUpdate.Invoke(newPosition); }
	inline void setScale(LLGP::Vector2<float> newScale) { m_Scale = newScale; };
	void setRotation(float newRotation);

	inline LLGP::Vector2<float> ReturnPosition() { return m_Position; }
	inline LLGP::Vector2<float> returnScale() { return m_Scale; }
	inline float returnRotation() { return m_Rotation; }

	LLGP::Event<LLGP::Vector2f> OnPositionUpdate;
private:
	LLGP::Vector2<float> m_Position;
	LLGP::Vector2<float> m_Scale;
	float m_Rotation;

};

