#pragma once
#include <SFML/Graphics.hpp>

class WindowManager
{
public:
	WindowManager() = delete;

	static void SetNewWindow(sf::Window* newWindow);

	static sf::Window* GetActiveWindow();

	static sf::Window* _ActiveWindow;
};
