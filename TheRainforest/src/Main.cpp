#include "Application.h"

#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) 
{			
																																		// SDL_WINDOW_MAXIMIZED
	std::unique_ptr<Application> app = std::make_unique<Application>("The Rainforest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_MAXIMIZED);
	SDL_ShowCursor(SDL_DISABLE);

	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	double delta;

	while (app->IsRunning())
	{
		last = now;
		now = SDL_GetPerformanceCounter();

		delta = static_cast<double>((now - last) / static_cast<double>(SDL_GetPerformanceFrequency()));

		app->OnEvent(delta);
		app->OnUpdate(delta);
		app->OnRender();
	}

	return 0;
}