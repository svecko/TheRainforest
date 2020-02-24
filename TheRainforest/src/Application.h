#pragma once

#include "FrameRateLimiter.h"

class Application : public FrameRateLimiter
{
public:
	Application();
	Application(const char* title, int x, int y, int w, int h, Uint32 flags);
	~Application();

	void OnEvent();
	void OnUpdate();
	void OnRender();

	bool IsRunning();

	void LimitFrameRate(int FPS) override;

private:
	bool m_IsRunning;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
};
