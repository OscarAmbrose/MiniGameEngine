#pragma once
#include <Vector2.h>
#include <SFML/Graphics.hpp>
#include <Component.h>
#include <GlobalEvents.h>
//#include <AssetManager.h>
#include<GlobalTexture.h>

//class Transform2D;
class GameObject;

class SpriteRenderer : public Component
{
public:

	SpriteRenderer(GameObject* owner);

	~SpriteRenderer()
	{
		g_OnRender.RemoveListener(this, std::bind(&SpriteRenderer::renderShape, this, std::placeholders::_1, std::placeholders::_2));
	}

	SpriteRenderer* setUV(LLGP::Vector2i selectedSprite);

	SpriteRenderer* setUV(LLGP::Vector2i selectedSprite, LLGP::Vector2i spriteSize);

	sf::RectangleShape returnShape();

	inline bool getFlipped() { return isFlipped; }
	void setFlipped(bool newFlipped);

	void renderShape(sf::RenderWindow* window, int renderLayer);

	void Update(float deltaTime)
	{
		Component::Update(deltaTime);
	}

	SpriteRenderer* SetRenderTexture(sf::Texture* text);

	SpriteRenderer* setOffSet(LLGP::Vector2f newOffset);
	inline LLGP::Vector2f getOffset() { return offset; }

	inline LLGP::Vector2<float> GetRectangleSize() { return m_RectangleSize; }

	SpriteRenderer* setRenderLayer(int newRenderLayer);
	inline int getRenderLayer() { return m_RenderLayer; }

private:
	sf::RectangleShape m_Shape;

	LLGP::Vector2<float> m_RectangleSize;

	LLGP::Vector2f offset = LLGP::Vector2f(0, 0);

	bool isFlipped = false;

	LLGP::Vector2i spritesInTexture = LLGP::Vector2i(32, 16);

	LLGP::Vector2i rectTexSize = LLGP::Vector2i(0, 0);

	int m_RenderLayer;
};
