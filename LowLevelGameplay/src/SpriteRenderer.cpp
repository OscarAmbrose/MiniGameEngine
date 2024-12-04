#include "SpriteRenderer.h"
#include <Transform.h>
#include <GameObject.h>
#include <Event.h>


SpriteRenderer::SpriteRenderer(GameObject* owner) : Component(owner)
{
	m_RenderLayer = 5;

	m_Shape.setTexture(renderTexture);

	rectTexSize = LLGP::Vector2i(m_Shape.getTexture()->getSize());

	//Vector2i selectedSprite = Vector2i(10, 15);
	LLGP::Vector2i selectedSprite = LLGP::Vector2i(0, 0);
	setUV(selectedSprite);

	g_OnRender.AddListener(this, std::bind(&SpriteRenderer::renderShape, this, std::placeholders::_1, std::placeholders::_2));
	g_OnStart.RemoveListener(this, std::bind(&Component::Start, this, std::placeholders::_1));
	g_OnFixedUpdate.RemoveListener(this, std::bind(&Component::FixedUpdate, this, std::placeholders::_1));
}

SpriteRenderer* SpriteRenderer::setUV(LLGP::Vector2i selectedSprite)
{
	m_RectangleSize = LLGP::Vector2<float>(32, 64);
	sf::IntRect rectTexUV((rectTexSize.x / spritesInTexture.x) * selectedSprite.x, (rectTexSize.y / spritesInTexture.y) * selectedSprite.y, rectTexSize.x / spritesInTexture.x, rectTexSize.y / spritesInTexture.y);
	m_Shape.setTextureRect(rectTexUV);
	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);

	return this;
}

SpriteRenderer* SpriteRenderer::setUV(LLGP::Vector2i selectedSprite, LLGP::Vector2i spriteSize)
{
	m_RectangleSize = LLGP::Vector2<float>(spriteSize.x, spriteSize.y);
	sf::IntRect rectTexUV((rectTexSize.x / spritesInTexture.x) * selectedSprite.x, (rectTexSize.y / spritesInTexture.y) * selectedSprite.y, spriteSize.x, spriteSize.y);
	m_Shape.setTextureRect(rectTexUV);
	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);

	return this;
}

sf::RectangleShape SpriteRenderer::returnShape()
{
	m_Shape.setRotation(m_GameObject->GetTransform()->returnRotation());

	LLGP::Vector2f position = m_GameObject->GetTransform()->ReturnPosition() + offset;

	m_Shape.setPosition(position);

	return m_Shape;
}

void SpriteRenderer::setFlipped(bool newFlipped)
{

	isFlipped = newFlipped;
	float x = m_GameObject->GetTransform()->returnScale().x;
	float y = m_GameObject->GetTransform()->returnScale().y;
	if (newFlipped)
	{
		m_Shape.setScale(-x, x);
	}
	else
	{
		m_Shape.setScale(x, x);
	}

}

void SpriteRenderer::renderShape(sf::RenderWindow* window, int renderLayer)
{
	if (!m_GameObject->GetActive()) { return; }
	if (renderLayer == getRenderLayer())
	{
		window->draw(returnShape());
	}
}

SpriteRenderer* SpriteRenderer::SetRenderTexture(sf::Texture* text)
{
	m_Shape.setTexture(text);
	return this;
}

SpriteRenderer* SpriteRenderer::setOffSet(LLGP::Vector2f newOffset)
{
	offset = newOffset;
	return this;
}

SpriteRenderer* SpriteRenderer::setRenderLayer(int newRenderLayer)
{
	m_RenderLayer = newRenderLayer;
	return this;
}
