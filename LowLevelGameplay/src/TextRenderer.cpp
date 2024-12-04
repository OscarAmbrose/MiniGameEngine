#include "TextRenderer.h"
#include <GameObject.h>
#include <Transform.h>

sf::Font* FontValues::mainFont;
FontValues* FontValues::instance;

TextRenderer::TextRenderer(GameObject* owner) : Component(owner)
{
	m_TextObject.setFont(*FontValues::mainFont);
	m_TextObject.setCharacterSize(m_CharacterSize);
	m_TextObject.setFillColor(m_TextColour);
	m_TextObject.setString(m_Text);
	g_OnRender.AddListener(this, std::bind(&TextRenderer::RenderText, this, std::placeholders::_1, std::placeholders::_2));
	m_GameObject->GetTransform()->OnPositionUpdate.AddListener(this, std::bind(&TextRenderer::SetPosition, this, std::placeholders::_1));
	SetPosition(m_GameObject->GetTransform()->ReturnPosition());
}

TextRenderer::~TextRenderer()
{
	g_OnRender.RemoveListener(this, std::bind(&TextRenderer::RenderText, this, std::placeholders::_1, std::placeholders::_2));
	//m_GameObject->GetTransform()->OnPositionUpdate.RemoveListener(this, std::bind(&TextRenderer::SetPosition, this, std::placeholders::_1));
}

TextRenderer* TextRenderer::SetTextFont(sf::Font newFont)
{
	m_TextObject.setFont(newFont);
	return this;
}

TextRenderer* TextRenderer::SetPosition(LLGP::Vector2f position)
{
	m_Position = position;
	m_TextObject.setPosition(position + m_Offset);
	return this;

}

TextRenderer* TextRenderer::SetOffset(LLGP::Vector2f newOffset)
{
	m_Offset = newOffset;
	m_TextObject.setPosition(m_Position + newOffset);
	return this;

}

TextRenderer* TextRenderer::SetText(std::string newText)
{
	m_Text = newText;
	m_TextObject.setString(newText);
	return this;
}

TextRenderer* TextRenderer::SetTextSize(int newFontSize)
{
	m_CharacterSize = newFontSize;
	m_TextObject.setCharacterSize(newFontSize);
	return this;
}

void TextRenderer::RenderText(sf::RenderWindow* window, int renderLayer)
{
	if (!m_GameObject->GetActive()) { return; }
	if (renderLayer == m_RenderLayer)
	{
		window->draw(m_TextObject);
	}
}

TextRenderer* TextRenderer::SetColour(sf::Color newColour)
{
	m_TextColour = newColour;
	m_TextObject.setFillColor(newColour);
	return this;
}
