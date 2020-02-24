#include "FrameRateLimiter.h"

FrameRateLimiter::FrameRateLimiter()
	: m_FPS(60), m_FrameDelay(1000 / m_FPS)
{
	m_FrameStart = SDL_GetTicks();
	m_FrameTime = SDL_GetTicks() - m_FrameStart;
}

FrameRateLimiter::~FrameRateLimiter()
{
}
