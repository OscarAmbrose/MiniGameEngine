#include "WindowManager.h"

sf::Window* WindowManager::_ActiveWindow;

void WindowManager::SetNewWindow(sf::Window* newWindow)
{
	delete _ActiveWindow;
	_ActiveWindow = newWindow;
}

sf::Window* WindowManager::GetActiveWindow()
{
	return _ActiveWindow;
}
