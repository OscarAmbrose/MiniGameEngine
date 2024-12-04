#pragma once
#include "DebugBox.h"
#include <GameObject.h>
#include <SpriteRenderer.h>
#include <BoxCollider.h>
#include <Transform.h>

DebugBox::DebugBox(GameObject* owner) : Component(owner)
{
	m_Shape.setFillColor(sf::Color::Transparent);
	m_Shape.setOutlineColor(sf::Color::Red);
	m_Shape.setOutlineThickness(3.f);

	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);
}


DebugBox::~DebugBox()
{
	g_OnRender.RemoveListener(this, std::bind(&DebugBox::renderShape, this, std::placeholders::_1, std::placeholders::_2));
}

DebugBox* DebugBox::SetUpDebugBox()
{
	m_RectangleSize = m_GameObject->GetComponent<BoxCollider>()->GetBoxSize();
	m_RenderLayer = 4;
	m_Shape.setFillColor(sf::Color::Transparent);
	m_Shape.setOutlineColor(sf::Color::Red);
	m_Shape.setOutlineThickness(1.f);

	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);

	
	g_OnRender.AddListener(this, std::bind(&DebugBox::renderShape, this, std::placeholders::_1, std::placeholders::_2));

	return this;
}

DebugBox* DebugBox::SetUpDebugBox(BoxCollider* boxCollider)
{
	m_PositionIsHandled = true;
	m_RectangleSize = boxCollider->GetBoxSize();
	m_UnsafeBoxCollider = boxCollider;
	m_RenderLayer = 4;
	m_Shape.setFillColor(sf::Color::Transparent);
	m_Shape.setOutlineColor(sf::Color::Red);
	m_Shape.setOutlineThickness(1.f);

	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);


	g_OnRender.AddListener(this, std::bind(&DebugBox::renderShape, this, std::placeholders::_1, std::placeholders::_2));

	return this;
}

void DebugBox::renderShape(sf::RenderWindow* window, int renderLayer)
{
	LLGP::Vector2f position;
	if (!m_PositionIsHandled) 
	{ 
		position = m_GameObject->GetComponent<BoxCollider>()->GetBoxPosition() + m_Offset;
	}
	else
	{
		position = m_UnsafeBoxCollider->GetBoxPosition();
	}
	
	m_Shape.setPosition(position);

	m_Shape.setSize(m_RectangleSize);
	m_Shape.setOrigin(m_RectangleSize / 2);


	if (renderLayer == m_RenderLayer)
	{
		//if (_GameObject->CompareTag("Player")) { std::cout << "DrawDebugBox\n"; }
		

		window->draw(m_Shape);
	}
}


