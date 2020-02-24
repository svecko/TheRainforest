#include "Application.h"
#include "Texture2D.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <memory>
#include <vector>

std::vector<std::unique_ptr<Texture2D>> tilemap;
std::vector<std::unique_ptr<Texture2D>> trees;
std::vector<std::unique_ptr<Texture2D>> fire;
std::vector<std::unique_ptr<Texture2D>> smoke;
std::unique_ptr<Texture2D> player;

static int s_PlayerX = 1920 / 2, s_PlayerY = 1080 / 2;
static int s_TargetX = 1920 / 2, s_TargetY = 1080 / 2;

Application::Application()
{
}

Application::Application(const char* title, int x, int y, int w, int h, Uint32 flags)
	: m_IsRunning(true)
{
	srand(time(nullptr));

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		m_Window = SDL_CreateWindow(title, x, y, w, h, flags);
		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
	}
	else
	{
		m_IsRunning = false;
	}

	tilemap.reserve(30 * 18);
	trees.reserve(6 * 3);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			int randomTile = rand() % 7;

			switch (randomTile)
			{
			case 0:
				tilemap.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 16, 32, i * 64, j * 64, 4.0f)));
				break;
			case 1:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 32, 32, i * 64, j * 64, 4.0f)));
				break;
			case 2:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 48, 32, i * 64, j * 64, 4.0f)));
				break;
			case 3:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 64, 32, i * 64, j * 64, 4.0f)));
				break;
			case 4:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 80, 32, i * 64, j * 64, 4.0f)));
				break;
			case 5:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 96, 32, i * 64, j * 64, 4.0f)));
				break;
			case 6:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 128, 32, i * 64, j * 64, 4.0f)));
				break;
			}
		}
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (rand() % 6 != 0)
			{
				trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/tileset/JungleTileset.png", m_Renderer, 80, 96, 240, 80, i * 328, j * 360, 3.5f)));
				if (rand() % 3 == 0)
				{
					fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/effect/Fire.png", m_Renderer, 50, 50, 0, 0, i * 328 - 75, j * 360 - 40, 8.0f)));
					smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>("assets/textures/effect/Smoke.png", m_Renderer, 50, 50, 0, 0, i * 328 - 25, j * 360 - 20, 6.0f)));
				}
			}
		}
	}

	player = std::make_unique<Texture2D>("assets/textures/sprite/Hero.png", m_Renderer, 16, 16, 16, 32, s_PlayerX, s_PlayerY, 3.5f);
}

Application::~Application()
{
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	SDL_Quit();
}

void Application::OnEvent()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				s_TargetY -= 15;
				player->Animate(16, 3, 100);
				break;
			case SDLK_s:
				s_TargetY += 15;
				player->Animate(32, 3, 100);
				break;
			case SDLK_a:
				s_TargetX -= 15;
				player->Animate(0, 3, 100);
				break;
			case SDLK_d:
				s_TargetX += 15;
				player->Animate(0, 3, 100, true);
				break;
			default:
				if (trees.size() != 0)
				{
					trees.pop_back();
				}
				break;
			}
		default:
			break;
		}
	}
}

void Application::OnUpdate()
{
	s_PlayerX = s_PlayerX + ((s_TargetX - s_PlayerX) * 0.08);
	s_PlayerY = s_PlayerY + ((s_TargetY - s_PlayerY) * 0.08);

	player->SetPosition(s_PlayerX, s_PlayerY);

	for (auto& particle : smoke)
	{
		particle->Animate(0, 8, 75);
	}

	for (auto& flame : fire)
	{
		flame->Animate(0, 8, 75);
	}
}

void Application::OnRender()
{
	SDL_RenderClear(m_Renderer);
	// Render here
	for (auto& tile : tilemap)
	{
		tile->Render();
	}

	player->Render();

	for (auto& particle : smoke)
	{
		particle->Render();
	}

	for (auto& tree : trees)
	{
		tree->Render();
	}

	for (auto& flame : fire)
	{
		flame->Render();
	}

	SDL_RenderPresent(m_Renderer);
}

bool Application::IsRunning()
{
	return m_IsRunning;
}

void Application::LimitFrameRate(int FPS)
{
	m_FPS = FPS;
	m_FrameDelay = 1000 / m_FPS;

	m_FrameStart = SDL_GetTicks();
	m_FrameTime = SDL_GetTicks() - m_FrameStart;

	if (m_FrameDelay > m_FrameTime)
	{
		SDL_Delay(m_FrameDelay - m_FrameTime);
	}
}
