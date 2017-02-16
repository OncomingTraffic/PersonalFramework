#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <SDL.h>


class GameTimer
{
	Uint32 m_StartTicks = 0;
	Uint32 m_PausedTicks = 0;

	float m_FrameRate;


public:
	GameTimer(float FrameRate = 60.0f);

	void StartFrameTimer();
	void PauseTimer();

	// Limits the frame rate and returns the time that has passed (Delta time)
	float LimitFrameRate();

	void SetFrameRate(float frameRate);
};



#endif GAMETIMER_H