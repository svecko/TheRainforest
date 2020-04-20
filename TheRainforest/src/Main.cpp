#include "Application.h"

#include <iostream>
#include <memory>

int main(int argc, char* argv[]) 
{																																		       // SDL_WINDOW_MAXIMIZED
	std::unique_ptr<Application> app = std::make_unique<Application>("The Rainforest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_ShowCursor(SDL_DISABLE);

	while (app->IsRunning())
	{
		app->LimitFrameRate(60);
		app->OnEvent();
		app->OnUpdate();
		app->OnRender();
	}

	return 0;
}