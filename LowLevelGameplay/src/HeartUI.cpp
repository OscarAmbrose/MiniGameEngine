#include "HeartUI.h"
#include <Transform.h>
#include <SpriteRenderer.h>
#include <TextRenderer.h>

HeartUI::HeartUI() : GameObject()
{
	SetMaxLives(3);
	SetHeartOffsets();
	AddComponent<TextRenderer>()->SetOffset(LLGP::Vector2f(-48.f, 34.f));
	SetPersistent(true);
}

HeartUI::~HeartUI()
{

}

HeartUI* HeartUI::SetMaxLives(int newLives, bool isFilling)
{
	for (int renderIndex = renderers.size()-1; renderIndex > newLives; renderIndex--)
	{
		RemoveComponent<SpriteRenderer>();
	}

	MaxLives = newLives;

	if (MaxLives == 0 || isFilling == false) { return this; }

	for (int spriteNum = 0; spriteNum < MaxLives; spriteNum++)
	{
		AddComponent<SpriteRenderer>()->setUV(fullHeartLocation, LLGP::Vector2i(89, 34))->setRenderLayer(3);
	}
	SetFilledHearts();
	return this;
}

HeartUI* HeartUI::SetMaxLives(int newLives)
{
	for (int renderIndex = renderers.size() - 1; renderIndex > newLives; renderIndex--)
	{
		RemoveComponent<SpriteRenderer>();
	}
	
	MaxLives = newLives;

	if (MaxLives == 0) { return this; }
	
	for (int spriteNum = 0; spriteNum < MaxLives; spriteNum++)
	{
		AddComponent<SpriteRenderer>()->setUV(fullHeartLocation)->setRenderLayer(3);
	}
	SetFilledHearts();
	return this;
}

void HeartUI::SetFilledHearts()
{
	for (int heartIndex = renderers.size() - 1; heartIndex >= 0; heartIndex--)
	{
		if ((heartIndex + 1) > CurrentLives)
		{
			renderers[heartIndex]->setUV(emptyHeartLocation);
		}
		else
		{
			renderers[heartIndex]->setUV(fullHeartLocation);
		}
	}
}

void HeartUI::SetHeartOffsets()
{
	renderers.clear();
	renderers = GetComponents<SpriteRenderer>();
	if (renderers.size() <= 0) { return; }
	float numberOfHearts = renderers.size();
	StepDistance = renderers[0]->GetRectangleSize().x;
	TotalSize = numberOfHearts * StepDistance;
	for (int renderIndex = 0; renderIndex < numberOfHearts; renderIndex++)
	{
		float offsetAmountX = (renderIndex - std::floor(numberOfHearts / 2.f)) * StepDistance;
		renderers[renderIndex]->setOffSet(LLGP::Vector2f(offsetAmountX, 0));
	}
}
