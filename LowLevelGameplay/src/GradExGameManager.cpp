#pragma once
#include <GradExGameManager.h>
#include <BoxCollider.h>
#include <Transform.h>
#include <Platform.h>
#include <Cursor.h>
#include <GradExPlayer.h>
#include <Projectile.h>
#include <SpriteRenderer.h>
#include <Timer.h>
#include <HealthComponent.h>
#include <string>
#include <functional>
#include <memory>
#include <HeartUI.h>
#include <TextRenderer.h>
#include <MapObject.h>
#include <GradExPlayerController.h>
#include <Weapon.h>
#include <RespawnManager.h>
#include <ObjectPool.h>

GradExGameManager::GradExGameManager() : GameManager()
{
	g_OnCollectGarbage.AddListener(this, std::bind(&GradExGameManager::CollectGarbage, this, std::placeholders::_1));
}

GradExGameManager::~GradExGameManager()
{
	g_OnCollectGarbage.RemoveListener(this, std::bind(&GradExGameManager::CollectGarbage, this, std::placeholders::_1));
}

void GradExGameManager::RespawnPlayer(int playerIndex)
{
	std::string playerName = "Player"+std::to_string(playerIndex);
	if (auto object = GetGameObjectByName(playerName)) 
	{ 
		if (object->GetActive() == true) { return; }
		object->GetComponent<HealthComponent>()->Respawn(); 
	}
}

void GradExGameManager::StartLevel(int levelNumber)
{
	ResetLevelState();
	for (PlayerCharacter* player : getAllObjectsOfType<PlayerCharacter>())
	{
		auto weapon = player->GetComponent<Weapon>();
		weapon->SetWeaponType(levelNumber);
	}
	TogglePlayerMovement(true);
	switch (levelNumber)
	{
		case 1:
		{

			break;
		}
		case 2:
		{

			break;
		}
		default:
			break;
	}

}

Timer* GradExGameManager::StartTimer(float timerLength, std::function<void(Timer*, int)> inFunc)
{
	Timer* newTimer = new Timer(timerLength);
	newTimer->TimerFinished.AddListener(this, inFunc);
	newTimer->StartTimer(timerLength);
	m_ActiveTimers.push_back(std::move(newTimer));
	return (m_ActiveTimers[m_ActiveTimers.size() - 1]);
}

void GradExGameManager::EndTimer(Timer* finishedTimer)
{
	m_DestroyableTimers.push_back(finishedTimer);
}

void GradExGameManager::ResetLevelState()
{
	m_Player0Lives = 3;
	m_Player1Lives = 3;
	UpdateHeartUI(0, 3);
	UpdateHeartUI(1, 3);
}

void GradExGameManager::ResetPlayerPositions()
{
	GetGameObjectByName("Player0")->GetTransform()->SetPosition(m_Player0StartLocation);
	GetGameObjectByName("Player1")->GetTransform()->SetPosition(m_Player1StartLocation);
}

void GradExGameManager::TogglePlayerMovement(bool newEnabled)
{
	if (m_WaitingForNextLevel) { return; }
	if (newEnabled)
	{
		if (GameObject* obj = GetGameObjectByName("Player0"))
		{
			if (PlayerController* pc = obj->GetComponent<PlayerController>()) { return; }
		}
		else { return; }
		GetGameObjectByName("Player0")->AddComponent<PlayerController>();
		GetGameObjectByName("Player1")->AddComponent<PlayerController>();
	}
	else
	{
		if (GameObject* obj = GetGameObjectByName("Player0"))
		{
			if (PlayerController* pc = obj->GetComponent<PlayerController>()) 
			{ 
				pc->m_ShouldDisableWeapon = true;
			}
			GameObject* obj2 = GetGameObjectByName("Player1");
			if (PlayerController* pc = obj2->GetComponent<PlayerController>())
			{
				pc->m_ShouldDisableWeapon = true;
			}
		}
		GetGameObjectByName("Player0")->RemoveComponent<PlayerController>();
		GetGameObjectByName("Player1")->RemoveComponent<PlayerController>();
	}
}

void GradExGameManager::NextLevelStarted(Timer* timer, int required)
{
	if(timer){ EndTimer(timer); }
	if (m_GameOver) { return; }
	m_WinTextContainer->SetActive(false);
	m_WaitingForNextLevel = false;
	m_CurrentLevel++;
	TogglePlayerMovement(true);
	RefillPlayerWeapons();
	ResetLevelState();
	if (m_CurrentLevel <= 5)
	{
		StartLevel(m_CurrentLevel);
	}
}

void GradExGameManager::ResetGame(Timer* timer, int required)
{
	EndTimer(timer);
	m_GameOver = false;
	NextLevelStarted(nullptr, 0);
}

void GradExGameManager::RefillPlayerWeapons()
{
	for (PlayerCharacter* pc : getAllObjectsOfType<PlayerCharacter>())
	{
		pc->GetComponent<Weapon>()->RefillAmmo(true);
	}
}

void GradExGameManager::PlayerDied(int playerNumber)
{
	ResetPlayerPositions();
	TogglePlayerMovement(false);
	switch (playerNumber)
	{
	case 0:
		m_Player0Lives--;
		UpdateHeartUI(playerNumber, m_Player0Lives);
		break;
	case 1:
		m_Player1Lives--;
		UpdateHeartUI(playerNumber, m_Player1Lives);
		break;
	default:
		break;
	}

	if (!LivesRemain())
	{
		LevelOver();
	}

	
}

void GradExGameManager::UpdateHeartUI(int playerNumber, int newLives)
{
 	std::string HeartUIName = "HeartUI" + std::to_string(playerNumber);
	{
		if (HeartUI* heartUI = static_cast<HeartUI*>(GetGameObjectByName(HeartUIName)))
		{
			heartUI->SetCurrentLives(newLives);
		}
	}
}

int GradExGameManager::GetPlayerLives(int playerNumber)
{
	switch (playerNumber)
	{
	case 0:
		return m_Player0Lives;
		break;
	case 1:
		return m_Player1Lives;
		break;
	default:
		break;
	}
}

void GradExGameManager::initialiseGame()
{
	AddGameObject(MapObject::NewMapBackground(LLGP::Vector2f(960, 540)));

#pragma region MakeMapColliders
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(-110, 0), LLGP::Vector2f(400, 74)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(290, -29), LLGP::Vector2f(1341, 49)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(1630, 0), LLGP::Vector2f(400, 74)));

	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(0, 280), LLGP::Vector2f(109, 520)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(504, 226), LLGP::Vector2f(109, 260)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(407, 747), LLGP::Vector2f(261, 108)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(884, 19), LLGP::Vector2f(109, 260)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(808, 486), LLGP::Vector2f(261, 108)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(884, 801), LLGP::Vector2f(109, 260)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(1264, 226), LLGP::Vector2f(109, 260)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(1209, 747), LLGP::Vector2f(261, 108)));
	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(1811, 280), LLGP::Vector2f(109, 520)));

	AddGameObject(MapObject::NewMapObject(LLGP::Vector2f(-100, 1061), LLGP::Vector2f(2120, 19)));
#pragma endregion

	GameObject* temp = AddGameObject<GameObject>()->SetName("WinContainer");
	temp->AddComponent<TextRenderer>()->SetColour(sf::Color::White)->SetTextSize(40)->SetText("");
	temp->SetPersistent(true)->SetActive(false);
	temp->GetTransform()->SetPosition(LLGP::Vector2f(960, 540));
	m_WinTextContainer = temp;

#pragma region CreateHeartUI
	AddGameObject<HeartUI>()->SetName("HeartUI0")->SetActive(true);
	GetGameObjectByName("HeartUI0")->GetTransform()->SetPosition(LLGP::Vector2f(64.f, 32.f));

	AddGameObject<HeartUI>()->SetName("HeartUI1")->SetActive(true);
	GetGameObjectByName("HeartUI1")->GetTransform()->SetPosition(LLGP::Vector2f(1856.f, 32.f));

	GameObject* testing = GetGameObjectByName("HeartUI0");
	GameObject* testing2 = GetGameObjectByName("HeartUI1");
	testing2->GetComponent<TextRenderer>()->SetText("Player 2");
#pragma endregion

	//Create player.
	AddGameObject<PlayerCharacter>()->SetPlayerNumber(0)->InitialiseCharacter()->SetName("Player0");
	GetGameObjectByName("Player0")->GetTransform()->SetPosition(m_Player0StartLocation);

	//Create second player.
	AddGameObject<PlayerCharacter>()->SetPlayerNumber(1)->InitialiseCharacter()->SetName("Player1");
	GetGameObjectByName("Player1")->GetTransform()->SetPosition(m_Player1StartLocation);
	GetGameObjectByName("Player1")->GetComponent<SpriteRenderer>()->setUV(LLGP::Vector2i(3, 3));
	GetGameObjectByName("Player1")->GetComponent<Cursor>()->m_SpriteRenderer->setUV(LLGP::Vector2i(11, 2));
}

bool GradExGameManager::LivesRemain()
{
	if (m_Player0Lives <= 0 || m_Player1Lives <= 0) { return false; }
	return true;
}

void GradExGameManager::LevelOver()
{
	std::cout << "level end\n";
	ClearGameObjects();
	m_WaitingForNextLevel = true;

	ObjectPooler::DebugDisableAllPoolObjects();

	std::string playerWins;
	sf::Color playerColour;
	if (m_Player0Lives > m_Player1Lives)
	{
		playerWins = "Player 1 Wins level " + std::to_string(m_CurrentLevel) + "!";
		playerColour = sf::Color::Blue;
		m_Player0Wins++;
	}
	else 
	{ 
		playerWins = "Player 2 Wins level " + std::to_string(m_CurrentLevel) + "!";
		playerColour = sf::Color::Red; 
		m_Player1Wins++;
	}
	size_t stringSize;
	stringSize = playerWins.size();

	if (m_Player1Wins >= 3 || m_Player0Wins >= 3)
	{
 		m_GameOver = true;
		playerWins = "Player " + std::to_string((m_Player1Wins > m_Player0Wins) + 1) + " Wins the game!\nCongratulations!\n\n\nThe Game will reset in 10 seconds";
	}


	if (m_GameOver) { stringSize = (("Player " + std::to_string((m_Player1Wins > m_Player0Wins) + 1) + " Wins the game!").size()); }

	//1/2 of text size
	float offset = 20;

	LLGP::Vector2f loc = LLGP::Vector2f(-(offset * stringSize), -offset);
	
	m_WinTextContainer->GetComponent<TextRenderer>()->SetOffset(loc);

	m_WinTextContainer->GetComponent<TextRenderer>()->SetText(playerWins)->SetColour(playerColour);
	m_WinTextContainer->SetActive(true);

	if (m_GameOver) 
	{
		m_Player0Wins = 0;
		m_Player1Wins = 0;
		m_CurrentLevel = 0;
		StartTimer(10.f, std::bind(&GradExGameManager::ResetGame, this, std::placeholders::_1, std::placeholders::_2));
		return; 
	}

	StartTimer(3.f, std::bind(&GradExGameManager::NextLevelStarted, this, std::placeholders::_1, std::placeholders::_2));
}

void GradExGameManager::CollectGarbage(float deltaTime)
{
	for (Timer* timer : m_DestroyableTimers)
	{
		bool timerFound = false;
		for (int timerIndex = 0; timerIndex < m_ActiveTimers.size(); timerIndex++)
		{
			if (m_ActiveTimers[timerIndex] == timer)
			{
				g_OnUpdate.RemoveListener(m_ActiveTimers[timerIndex], std::bind(&Timer::Update, m_ActiveTimers[timerIndex], std::placeholders::_1));
				m_ActiveTimers[timerIndex]->TimerFinished.Empty();
				delete m_ActiveTimers[timerIndex];
				m_ActiveTimers.erase(m_ActiveTimers.begin() + timerIndex);
				timerFound = true;
				break;
			}
		}
		if (!timerFound) { throw std::invalid_argument("Timer was not found, memory leak detected."); }
	}
	m_DestroyableTimers.erase(m_DestroyableTimers.begin(), m_DestroyableTimers.end());
	GameManager::CollectGarbage(deltaTime);
}

void GradExGameManager::Start()
{
	initialiseGame();
	StartLevel(1);
}
