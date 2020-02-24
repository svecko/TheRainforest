#pragma once

#include <SDL.H>

class FrameRateLimiter
{
public:
	FrameRateLimiter();
	virtual ~FrameRateLimiter();

	virtual void LimitFrameRate(int FPS) = 0;

protected:
	int m_FPS;
	int m_FrameDelay;
	Uint32 m_FrameStart;
	int m_FrameTime;
};
