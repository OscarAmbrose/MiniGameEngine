#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

extern sf::Texture* renderTexture;
extern sf::Texture* mapRenderTexture;

class GlobalTexture
{
public:
	GlobalTexture()
	{
		renderTexture = new sf::Texture();
		renderTexture->loadFromFile("Textures/JoustSpritesActual.png");
		mapRenderTexture = new sf::Texture();
		mapRenderTexture->loadFromFile("Textures/DefaultMap.png");
	}

	~GlobalTexture() {};
};
