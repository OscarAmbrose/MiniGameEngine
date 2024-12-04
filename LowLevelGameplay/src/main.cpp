#pragma once
#include <Object.h>
#include <GameObject.h>
#include <Component.h>
#include <Transform.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <GameObject.h>
#include <GlobalEvents.h>
#include <GradExGameManager.h>
#include <AnimationManager.h>
#include <Physics.h>
#include <WindowManager.h>
#include <ObjectPool.h>
#include <Projectile.h>
#include <cstdlib>
#include <ctime>

#include <GradExPlayer.h>
#include <Weapon.h>
#include <RespawnManager.h>
#include <TextRenderer.h>

#define FIXEDFRAMERATE (1.f/60.f)

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "GradEx 2024 ~ Oscar Ambrose", sf::Style::Fullscreen);

	WindowManager::SetNewWindow(&window);
	std::unique_ptr <FontValues> fontValues = std::make_unique<FontValues>();

	std::unique_ptr<GlobalTexture> globalTextureTest = std::make_unique<GlobalTexture>();
	
	std::unique_ptr <GradExGameManager> gameManager = std::make_unique <GradExGameManager>();
	

	ObjectPooler::InitialisePool();

	srand(static_cast<unsigned int>(time(0)));

	//Update Stuff
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.f;
	float timeSincePhysicsStep = 0.f;
	int numberOfFixedUpdates = 0;
	int numberOfUpdates = 0;
	//Update Stuff
	bool paused = false;
	

	//Creates the window
	
	GameManager::instance->Start();
	
	while (window.isOpen())
	{

		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 1000000.f;
		lastTime = now;



		#pragma region CloseWindow
		sf::Event event;
		while (window.pollEvent(event))
		{
			//std::cout << "Event polled: " << event.type << std::endl;

			if (event.type == sf::Event::Closed)
			{
				window.close();
				continue;
			}

			if (event.type == sf::Event::LostFocus)
			{
				paused = true;
				continue;
			}
			if (event.type == sf::Event::GainedFocus)
			{
				paused = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
				continue;
			}

			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (numberOfFixedUpdates > 20))
			//{
			//	auto gameManager = static_cast<GradExGameManager*>(GameManager::instance);

			//	gameManager->PlayerDied(0);

			//	numberOfFixedUpdates = 0;
			//}
			g_OnPollInputs(event);
		}

		if (paused)
		{
			deltaTime = 0.f;
			continue;
		}

		g_OnStart(0);
#pragma endregion

		//Input Handling
		numberOfUpdates++;
		

		#pragma region Physics
		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
			g_OnFixedUpdate(FIXEDFRAMERATE); 
			//step physics
			//collect collision info
			g_OnPhysicsUpdate(FIXEDFRAMERATE);

			Physics::CollectCollisions();
			
			//dispatch collisions
			Physics::DispatchCollisions();
			numberOfFixedUpdates++;
			timeSincePhysicsStep -= FIXEDFRAMERATE;

			if (numberOfFixedUpdates >= 30)
			{
				g_OnCollectGarbage(0.f);
				numberOfFixedUpdates = 0;
			}
		}
		#pragma endregion
		//Update

		g_OnUpdate(deltaTime);
		#pragma region Render
		window.clear();

		//I should change this to a static rendering manager class with bitmasking etc.
		for (int i = 0; i <= 5; i++)
		{
			g_OnRender(&window, i);
		}

		window.display();
#pragma endregion
	}

	return 0;
}
