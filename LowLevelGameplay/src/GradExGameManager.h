#pragma once
#include <GameManager.h>
#include <Event.h>
#include <Vector2.h>

class Timer;
class TextRenderer;

class GradExGameManager : public GameManager
{
public:
	GradExGameManager();
	~GradExGameManager();

	/// <summary>
	/// Call this function to respawn the player.
	/// </summary>
	/// <param name="playerIndex">The players index (controller number)</param>
	void RespawnPlayer(int playerIndex);

	/// <summary>
	/// Starts the level matching the inputed int. 
	/// </summary>
	/// <param name="levelNumber"></param>
	void StartLevel(int levelNumber);

	Timer* StartTimer(float timerLength, std::function<void(Timer*, int)> inFunc);

	void PlayerDied(int playerNumber);

	void UpdateHeartUI(int playerNumber, int newLives);

	int GetPlayerLives(int playerNumber);

	void CollectGarbage(float deltaTime) override;

	void RespawnPlayerZero(Timer* timer, int required)
	{
		EndTimer(timer);
		RespawnPlayer(0);
		TogglePlayerMovement(true);
	}
	void RespawnPlayerOne(Timer* timer, int required) 
	{
		EndTimer(timer);
		RespawnPlayer(1);
		TogglePlayerMovement(true);
	}
	/// <summary>
	/// See: <seealso cref="GameManager::Start"/> for details
	/// </summary>
	void Start() override;

	LLGP::Event<int> LevelStarted;
	LLGP::Event<int> m_RespawnPlayer;
private:
	std::vector<Timer*> m_ActiveTimers;
	std::vector<Timer*> m_DestroyableTimers;
	int m_Player0Lives = 3;
	int m_Player1Lives = 3;
	
	LLGP::Vector2f m_Player0StartLocation = LLGP::Vector2f(200, 540);
	LLGP::Vector2f m_Player1StartLocation = LLGP::Vector2f(1720, 540);

	bool m_WaitingForNextLevel = false;

	int m_CurrentLevel = 1;

	int m_Player0Wins = 0;
	int m_Player1Wins = 0;

	bool m_GameOver = false;

private:
	void initialiseGame();
	bool LivesRemain();
	void LevelOver();
	void EndTimer(Timer* finishedTimer);
	void ResetLevelState();
	void ResetPlayerPositions();
	void TogglePlayerMovement(bool newEnabled);
	void NextLevelStarted(Timer* timer, int required);
	void ResetGame(Timer* timer, int required);
	GameObject* m_WinTextContainer;
	void RefillPlayerWeapons();
};
