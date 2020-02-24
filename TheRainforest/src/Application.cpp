#include "Application.h"
#include "Texture2D.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <memory>
#include <vector>

std::vector<Texture2D*> tilemap;
std::vector<Texture2D*> trees;
Texture2D* player;

static int s_X = 1920 / 2, s_Y = 1080 / 2;

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

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			int randomTile = rand() % 7;

			switch (randomTile)
			{
			case 0:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 16, 32, i * 64, j * 64, 4.0f));
				break;
			case 1:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 32, 32, i * 64, j * 64, 4.0f));
				break;
			case 2:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 48, 32, i * 64, j * 64, 4.0f));
				break;
			case 3:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 64, 32, i * 64, j * 64, 4.0f));
				break;
			case 4:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 80, 32, i * 64, j * 64, 4.0f));
				break;
			case 5:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 96, 32, i * 64, j * 64, 4.0f));
				break;
			case 6:
				tilemap.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 16, 16, 128, 32, i * 64, j * 64, 4.0f));
				break;
			}
		}
	}

	for (int i = 0; i < 6; i++)
	{
		float x = (rand() % 3 + 1) * 1.1;
		trees.push_back(new Texture2D("assets/textures/tileset/JungleTileset.png", m_Renderer, 80, 96, 240, 80, i * 320, sin(i * x) * 300 + 400, 4.0f));
	}

	player = new Texture2D("assets/textures/sprite/Hero.png", m_Renderer, 16, 16, 16, 32, s_X, s_Y, 4.0f);
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
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_w)
			{
				s_Y -= 1 * 10;
				player->Render();
			}
			else if (e.key.keysym.sym == SDLK_s)
			{
				s_Y += 1 * 10;
			}
			else if (e.key.keysym.sym == SDLK_a)
			{
				s_X -= 1 * 10;
			}
			else if (e.key.keysym.sym == SDLK_d)
			{
				s_X += 1 * 10;
			}
		default:
			break;
		}
	}
}

void Application::OnUpdate()
{
	player->SetPosition(s_X, s_Y);
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

	for (auto& tree : trees)
	{
		tree->Render();
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
