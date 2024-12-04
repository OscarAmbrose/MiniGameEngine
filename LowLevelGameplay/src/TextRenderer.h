#pragma once
#include <Component.h>
#include <SFML/Graphics.hpp>
#include <Vector2.h>
#include <GlobalEvents.h>

class TextRenderer : public Component
{
public:
	TextRenderer(GameObject* owner);
	~TextRenderer();

public:
	TextRenderer* SetTextFont(sf::Font newFont);
	TextRenderer* SetPosition(LLGP::Vector2f position);
	TextRenderer* SetOffset(LLGP::Vector2f position);
	void RenderText(sf::RenderWindow* window, int renderLayer);
	TextRenderer* SetColour(sf::Color newColour);
	TextRenderer* SetText(std::string newText);
	TextRenderer* SetTextSize(int newFontSize);

protected:
	sf::Text m_TextObject;
	std::string m_Text = "Player 1";
	int m_CharacterSize = 16;
	sf::Color m_TextColour = sf::Color::White;
	LLGP::Vector2f m_Position = LLGP::Vector2f::zero;
	LLGP::Vector2f m_Offset = LLGP::Vector2f::zero;
	int m_RenderLayer = 4;
};

class FontValues
{
public:
	FontValues()
	{
		if (FontValues::instance != nullptr)
		{
			delete this;
			return;
		}
		FontValues::instance = this;

		FontValues::mainFont = (new sf::Font());
		if (!FontValues::mainFont->loadFromFile("Fonts/joystix monospace.otf"))
		{
			std::cerr << "Error loading font\n";
		}
	}
	~FontValues() 
	{ 
		delete FontValues::mainFont; 
	}

	static sf::Font* mainFont;
	static FontValues* instance;
};



