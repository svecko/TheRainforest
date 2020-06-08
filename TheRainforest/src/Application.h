#pragma once

#include <SDL.h>

class Application
{
public:
	Application();
	Application(const char* title, int x, int y, int w, int h, Uint32 flags);
	~Application();

	void OnEvent(double delta);
	void OnUpdate(double delta);
	void OnRender();

	bool IsRunning();

private:
	bool m_IsRunning;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
};
