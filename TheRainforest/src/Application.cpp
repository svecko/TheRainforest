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
std::vector<std::unique_ptr<Texture2D>> hotbar;
std::vector<std::unique_ptr<Texture2D>> enemies;
std::unique_ptr<Texture2D> player;
std::unique_ptr<Texture2D> cursor;
SDL_Texture* terrainTexture;
SDL_Texture* fireTexture;
SDL_Texture* hotbarTexture;
SDL_Texture* enemyTexture;

const Uint8* keyState;

static int s_MouseX, s_MouseY;
static int s_TargetMouseX = 0, s_TargetMouseY = 0;
static int s_PlayerX = 1920 / 2, s_PlayerY = 1080 / 2;
static int s_TargetX = 1920 / 2, s_TargetY = 1080 / 2;
static Vector2D s_PlayerVelocity{ 0, 0 };

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

	terrainTexture = TextureLoader::LoadTexture("assets/textures/tileset/Terrain.png", m_Renderer);
	fireTexture = TextureLoader::LoadTexture("assets/textures/effect/Flames.png", m_Renderer);
	hotbarTexture = TextureLoader::LoadTexture("assets/textures/ui/Hotbar.png", m_Renderer);
	enemyTexture = TextureLoader::LoadTexture("assets/textures/sprite/Thief.png", m_Renderer);

	tilemap.reserve(30 * 18);
	trees.reserve(6 * 3);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			int randomTile = rand() % 8;
			int randomTree;

			switch (randomTile)
			{
			case 0:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 80, 0, i * 64, j * 64, 4.0f, "Tile")));
				break;
			case 1:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 96, 0, i * 64, j * 64, 4.0f, "Tile")));
				break;
			case 2:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
				randomTree = rand() % 2;
				if (randomTree == 0)
				{
					trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 240, i * 64, j * 64, 4.0f, "Tree")));
				}
				else
				{
					trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 192, i * 64, j * 64, 4.0f, "Tree")));
				}
				break;
			default:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
				break;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		hotbar.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(hotbarTexture, m_Renderer, 16, 16, 0, 0, 1920 / 2 - 128 + i * 64, 1080 - 80, 4.0f, "Hotbar")));
	}

	for (int i = 0; i < 5; i++)
	{
		int randomX = rand() % 1920, randomY = rand() % 1080;
		enemies.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(enemyTexture, m_Renderer, 32, 32, 0, 0, randomX, randomY, 2.5f, "Enemy")));
	}

	for (auto& enemy : enemies)
	{
		int randomDir = rand() % 4;
		if (randomDir == 0)
		{
			enemy->dirY = 0;
			enemy->dirX = 1;
		}
		else if (randomDir == 1)
		{
			enemy->dirY = 0;
			enemy->dirX = -1;
		}
		else if (randomDir == 2)
		{
			enemy->dirX = 0;
			enemy->dirY = 1;
		}
		else if (randomDir == 3)
		{
			enemy->dirX = 0;
			enemy->dirY = -1;
		}
	}

	cursor = std::make_unique<Texture2D>("assets/textures/ui/Cursor.png", m_Renderer, 16, 16, 0, 0, 0, 0, 3.0f, "Cursor");
	// player = std::make_unique<Texture2D>("assets/textures/sprite/Hero.png", m_Renderer, 16, 16, 16, 32, 1920 / 2, 1080 / 2, 3.0f, "Player");
	player = std::make_unique<Texture2D>("assets/textures/sprite/TownMale.png", m_Renderer, 32, 32, 0, 0, 1920 / 2, 1080 / 2, 2.5f, "Player");
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
		default:
			break;
		}
	}

	keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W])
	{
		if (!keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
		{
			s_PlayerVelocity.y = -1;
			s_PlayerVelocity.x = 0;
		}
	}
	else if (keyState[SDL_SCANCODE_S])
	{
		if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
		{
			s_PlayerVelocity.y = 1;
			s_PlayerVelocity.x = 0;
		}
	}
	else if (keyState[SDL_SCANCODE_A])
	{
		if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_D])
		{
			s_PlayerVelocity.x = -1;
			s_PlayerVelocity.y = 0;
		}
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_A])
		{
			s_PlayerVelocity.x = 1;
			s_PlayerVelocity.y = 0;
		}
	}
	else
	{
		s_PlayerVelocity = { 0, 0 };
	}

	SDL_GetMouseState(&s_TargetMouseX, &s_TargetMouseY);
	s_MouseX = s_MouseX + ((s_TargetMouseX - s_MouseX) * 0.25);
	s_MouseY = s_MouseY + ((s_TargetMouseY - s_MouseY) * 0.25);

	for (auto& element : hotbar)
	{
		if (element->IsColliding(s_MouseX, s_MouseY))
		{
			element->SetSrcRect(16, 0);
		}
		else
		{
			element->SetSrcRect(0, 0);
		}
	}
}

void Application::OnUpdate()
{
	m_FrameCount++;
	cursor->SetPosition(s_MouseX, s_MouseY);

	if (s_PlayerVelocity.x == 1)
	{
		s_TargetX = s_PlayerX + 75;
		player->Animate(0, 4, 100);
	}
	else if (s_PlayerVelocity.x == -1)
	{
		s_TargetX = s_PlayerX - 75;
		player->Animate(0, 4, 100, true);
	}
	else if (s_PlayerVelocity.y == 1)
	{
		s_TargetY = s_PlayerY + 75;
		player->Animate(0, 4, 100);
	}
	else if (s_PlayerVelocity.y == -1)
	{
		s_TargetY = s_PlayerY - 75;
		player->Animate(0, 4, 100);
	}
	else
	{
		player->Animate(32, 4, 100);
	}

	for (auto& enemy : enemies)
	{
		enemy->SetPosition(enemy->GetPosition().x + 1, enemy->GetPosition().y + 1);
	}

	for (auto& particles : smoke)
	{
		particles->Animate(0, 2, 100, true);
	}

	for (auto& flame : fire)
	{
		flame->Animate(32, 4, 100);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		for (auto& tree : trees)
		{
			if (enemies[i]->IsColliding(*tree))
			{
				tree->SetIsLit(true);
				int randomX = rand() % 1920, randomY = rand() % 1080;
				enemies.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(enemyTexture, m_Renderer, 32, 32, 0, 0, randomX, randomY, 2.5f, "Enemy")));
				enemies.erase(enemies.begin() + i);
				fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 32, tree->GetPosition().x, tree->GetPosition().y - 16, 4.0f, "Fire")));
				smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, tree->GetPosition().x, tree->GetPosition().y - 16, 4.0f, "Smoke")));
			}
		}
		enemies[i]->Animate(32, 4, 100);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->IsColliding(*player))
		{
			enemies.erase(enemies.begin() + i);
		}
	}

   for (int i = 0; i < fire.size(); i++)
   {
	   if (player->IsColliding(*fire[i]))
	   {
		   fire.erase(fire.begin() + i);
		   smoke.erase(smoke.begin() + i);
	   }
   }

	// s_PlayerX += s_PlayerVelocity.x * 4;
	// s_PlayerY += s_PlayerVelocity.y * 4;

	s_PlayerX += s_PlayerVelocity.x + ((s_TargetX - s_PlayerX) * 0.05);
	s_PlayerY += s_PlayerVelocity.y + ((s_TargetY - s_PlayerY) * 0.05);

	player->SetPosition(s_PlayerX, s_PlayerY);

}

void Application::OnRender()
{
	SDL_RenderClear(m_Renderer);
	// Render here
	for (auto& tile : tilemap)
	{
		tile->Render();
	}

	for (auto& tree : trees)
	{
		tree->Render();
	}

	for (auto& particles : smoke)
	{
		particles->Render();
	}

	for (auto& flame : fire)
	{
		flame->Render();
	}

	player->Render();

	for (auto& enemy : enemies)
	{
		enemy->Render();
	}

	for (auto& element : hotbar)
	{
		element->Render();
	}

	cursor->Render();

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
