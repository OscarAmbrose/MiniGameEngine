#include <Timer.h>
#include <GlobalEvents.h>

Timer::Timer(float Timer) : Object()
{
	g_OnUpdate.AddListener(this, std::bind(&Timer::Update, this, std::placeholders::_1));
	_TimerEnded = true;
	_ElapsedTime = 0;
	_MaxTimer = Timer;
}

Timer::~Timer()
{
	TimerFinished.Empty();
	g_OnUpdate.RemoveListener(this, std::bind(&Timer::Update, this, std::placeholders::_1));
}



void Timer::Update(float deltaTime)
{
	if (_TimerEnded == true) { return; }
	_ElapsedTime += deltaTime;
	if (_ElapsedTime >= _MaxTimer)
	{
		EndTimer(true);
	}
}

void Timer::StartTimer(float deltaTime)
{
	_ElapsedTime = 0.f;
	_MaxTimer = deltaTime;
	_TimerEnded = false;
}

void Timer::ClearAndInvalidateTimer()
{
	EndTimer(false);
}

void Timer::EndTimer(bool norm)
{
	_TimerEnded = true;
	_ElapsedTime = 0.f;
	TimerFinished.Invoke(this, norm);
}
