#include "GameTimer.h"


GameTimer::GameTimer(float frameRate) : m_FrameRate(frameRate)
{

}

void GameTimer::StartFrameTimer()
{
	m_PausedTicks = 0;
	m_StartTicks = SDL_GetTicks();
}

void GameTimer::PauseTimer()
{
	m_PausedTicks = m_StartTicks;
}

float GameTimer::LimitFrameRate()
{
	float timePassed = SDL_GetTicks() - (m_StartTicks + m_PausedTicks);
	if (timePassed < 1000.0f / m_FrameRate)
	{
		SDL_Delay(1000.0f / m_FrameRate - timePassed);
	}
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Time Passed: %f", timePassed / 1000.0f);
	return timePassed / 1000.0f;
}

void GameTimer::SetFrameRate(float aFrameRate)
{
	m_FrameRate = aFrameRate;
}