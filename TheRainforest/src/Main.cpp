#include "Application.h"

#include <memory>

int main(int argc, char* argv[]) 
{
	std::unique_ptr<Application> app = std::make_unique<Application>("The Rainforest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_FULLSCREEN_DESKTOP);

	while (app->IsRunning())
	{
		app->LimitFrameRate(60);
		app->OnEvent();
		app->OnUpdate();
		app->OnRender();
	}

	return 0;
}