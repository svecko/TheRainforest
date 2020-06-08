#include "Application.h"
#include "Texture2D.h"
#include "File.h"
#include "Text.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <SDL_ttf.h>

#include <memory>
#include <vector>

std::vector<std::unique_ptr<Texture2D>> tilemap;
std::vector<std::unique_ptr<Texture2D>> trees;
std::vector<std::unique_ptr<Texture2D>> fire;
std::vector<std::unique_ptr<Texture2D>> smoke;
std::vector<std::unique_ptr<Texture2D>> hotbar;
std::vector<std::unique_ptr<Texture2D>> enemies;
std::vector<std::unique_ptr<Texture2D>> helpers;
std::unique_ptr<Texture2D> player;
std::unique_ptr<Texture2D> cursor;
std::unique_ptr<Texture2D> startTitle;
std::unique_ptr<Texture2D> startBackground;
std::unique_ptr<Texture2D> playButton;
std::unique_ptr<Texture2D> scoreButton;
std::unique_ptr<Texture2D> endScoreButton;
std::unique_ptr<Texture2D> startExitButton;
std::unique_ptr<Texture2D> pauseButton;
std::unique_ptr<Texture2D> pauseOverlay;
std::unique_ptr<Texture2D> endScoreOverlay;
std::unique_ptr<Texture2D> resumeButton;
std::unique_ptr<Texture2D> exitButton;
SDL_Texture* terrainTexture;
SDL_Texture* fireTexture;
SDL_Texture* hotbarTexture;
SDL_Texture* enemyTexture;
SDL_Texture* helperTexture;

static const float s_SPEED = 200;
static const float s_ENEMY_SPEED = 150;
static const float s_HELPER_SPEED = 100;
static const float s_FRICTION = 8; 
static int s_Level = 1;

const Uint8* keyState;

File* highScore, progress;
std::string s_PlayerName;
static int s_Score = 0;

static int s_MouseX, s_MouseY;
static bool s_MouseLeftIsPressed, s_MouseRightIsPressed;



enum Scene
{
	Start = 0, Pause, End, Level, Score
};


static Scene s_Scene = Start;

float RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

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
	helperTexture = TextureLoader::LoadTexture("assets/textures/sprite/Archer.png", m_Renderer);

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
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 80, 0, i * 64, j * 64, 4.0f, "GrassTile")));
				break;
			case 1:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 96, 0, i * 64, j * 64, 4.0f, "RoseTile")));
				break;
			case 2:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
				randomTree = rand() % 2;
				if (randomTree == 0)
				{
					trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 240, i * 64, j * 64, 4.0f, "Tree")));
					// smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, i * 64, j * 64 - 24, 4.0f, "Smoke")));
					// fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 32, i * 64, j * 64 - 16, 4.0f, "Fire")));
				}
				else
				{
					trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 192, i * 64, j * 64, 4.0f, "Tree")));
					// smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, i * 64, j * 64 - 24, 4.0f, "Smoke")));
					// fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 32, 32, i * 64, j * 64 - 16, 4.0f, "Fire")));
				}
				break;
			default:
				tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
				break;
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		enemies.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(enemyTexture, m_Renderer, 32, 32, 0, 0, 1920 / 2 - 32 * 1.5f, 1080 / 2 - 32 * 1.5f, 3.0f, "Enemy")));
	}

	for (int i = 0; i < 3; i++)
	{
		helpers.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(helperTexture, m_Renderer, 32, 32, 0, 0, RandomFloat(0, 1920 - 32 * 3), RandomFloat(0, 1080 - 32 * 3), 3.0f, "Helper")));
	}

	cursor = std::make_unique<Texture2D>("assets/textures/ui/Cursor.png", m_Renderer, 16, 16, 0, 0, 0, 0, 3.0f, "Cursor");
	player = std::make_unique<Texture2D>("assets/textures/sprite/TownMale.png", m_Renderer, 32, 32, 0, 0, 1920 / 2 - 32 * 1.5f, 1080 / 2 - 32 * 1.5f, 3.0f, "Player");
	startBackground = std::make_unique<Texture2D>("assets/textures/start/Background.png", m_Renderer, 480, 270, 0, 0, 0, 0, 4.0f, "Background");
	startTitle = std::make_unique<Texture2D>("assets/textures/start/TheRainforest.png", m_Renderer, 68, 8, 0, 0, 1920 / 2 - 68 * 8, 1080 / 2 - 8 * 8, 16.0f, "Background");
	playButton = std::make_unique<Texture2D>("assets/textures/start/Play.png", m_Renderer, 28, 11, 0, 0, 1920 / 2 - 28 * 4, 1080 / 2 - 11 * 4 + 200, 8.0f, "PlayButton");
	scoreButton = std::make_unique<Texture2D>("assets/textures/start/Score.png", m_Renderer, 32, 11, 0, 0, 1920 / 2 - 32 * 2, 1080 / 2 - 11 * 2 + 300, 4.0f, "ScoreButton");
	startExitButton = std::make_unique<Texture2D>("assets/textures/pause/Exit.png", m_Renderer, 26, 11, 0, 0, 1920 / 2 - 26 * 2, 1080 / 2 - 11 * 2 + 350, 4.0f, "StartExitButton");
	pauseButton = std::make_unique<Texture2D>("assets/textures/pause/Pause.png", m_Renderer, 32, 11, 0, 0, 1920 / 2 - 32 * 4, 1080 / 2 - 11 * 4, 8.0f, "PauseButton");
	pauseOverlay = std::make_unique<Texture2D>("assets/textures/pause/Overlay.png", m_Renderer, 480, 270, 0, 0, 0, 0, 4.0f, "PauseOverlay");
	resumeButton = std::make_unique<Texture2D>("assets/textures/pause/Resume.png", m_Renderer, 38, 11, 0, 0, 1920 / 2 - 38 * 2, 1080 / 2 - 11 * 2 + 100, 4.0f, "ResumeButton");
	exitButton = std::make_unique<Texture2D>("assets/textures/pause/Exit.png", m_Renderer, 26, 11, 0, 0, 1920 / 2 - 26 * 2, 1080 / 2 - 11 * 2 - 100, 4.0f, "ExitButton");
	endScoreButton = std::make_unique<Texture2D>("assets/textures/start/Score.png", m_Renderer, 32, 11, 0, 0, 1920 / 2 - 32 * 4, 1080 / 2 - 11 * 4 - 350, 8.0f, "ScoreButton");
	endScoreOverlay = std::make_unique<Texture2D>("assets/textures/score/Overlay.png", m_Renderer, 480, 270, 0, 0, 0, 0, 4.0f, "ScoreOverlay");


	for (auto& enemy : enemies)
	{
		enemy->SetVelocity(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
		enemy->SetVelocity(enemy->GetVelocity().Normalize());
	}

	for (auto& helper : helpers)
	{
		helper->SetVelocity(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
		helper->SetVelocity(helper->GetVelocity().Normalize());
	}
}

Application::~Application()
{
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	SDL_Quit();
}

void Application::OnEvent(double delta)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				s_MouseLeftIsPressed = true;
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				s_MouseRightIsPressed = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				s_MouseLeftIsPressed = false;
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				s_MouseRightIsPressed = false;
			}
			break;
		default:
			break;
		}
	}


	keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_ESCAPE] && s_Scene == Level)
	{
		s_Scene = Pause;
	}

	Vector2D inputVector = { 0.0f , 0.0f };
	inputVector.x = keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A];
	inputVector.y = keyState[SDL_SCANCODE_S] - keyState[SDL_SCANCODE_W];
	inputVector = inputVector.Normalize();

	switch (s_Scene)
	{
	case Start:
		break;
	case Pause:
		break;
	case End:
		break;
	case Level:

		for (auto& enemy : enemies)
		{
			if (!enemy->GetVelocity().IsZero())
			{
				if (enemy->GetVelocity().x > 0)
				{
					enemy->Animate(0, 4, 100);
				}
				else
				{
					enemy->Animate(0, 4, 100, true);
				}
			}
			else
			{
				enemy->Animate(32, 2, 200);
			}
		}

		for (auto& helper : helpers)
		{
			if (!helper->GetVelocity().IsZero())
			{
				if (helper->GetVelocity().x > 0)
				{
					helper->Animate(0, 4, 100);
				}
				else
				{
					helper->Animate(0, 4, 100, true);
				}
			}
			else
			{
				helper->Animate(32, 2, 200);
			}
		}

		if (!inputVector.IsZero())
		{
			if (inputVector.x > 0)
			{
				player->Animate(0, 4, 100);
			}
			else
			{
				player->Animate(0, 4, 100, true);
			}

			player->SetVelocity(inputVector);
		}
		else
		{
			player->Animate(32, 2, 200);
			player->SetVelocity(player->GetVelocity().MoveTowards({ 0, 0 }, s_FRICTION * delta));
		}
		break;
	case Score:
		break;
	default:
		break;
	}

	SDL_GetMouseState(&s_MouseX, &s_MouseY);
}

void Application::OnUpdate(double delta)
{
	cursor->SetPosition(s_MouseX, s_MouseY);

	switch (s_Scene)
	{
	case Start:
		if (playButton->IsColliding(s_MouseX, s_MouseY) && s_MouseLeftIsPressed)
		{
			s_Scene = Level;
		}
		if (scoreButton->IsColliding(s_MouseX, s_MouseY) && s_MouseLeftIsPressed)
		{
			s_Scene = Score;
		}
		if (startExitButton->IsColliding(s_MouseX, s_MouseY) && s_MouseLeftIsPressed)
		{
			m_IsRunning = false;
		}
		break;
	case Pause:
		if (resumeButton->IsColliding(s_MouseX, s_MouseY) && s_MouseLeftIsPressed)
		{
			s_Scene = Level;
		}
		if (exitButton->IsColliding(s_MouseX, s_MouseY) && s_MouseLeftIsPressed)
		{
			s_Scene = End;
		}
		break;
	case End:
		std::cout << "Enter your name: " << std::endl;
		std::getline(std::cin, s_PlayerName);
		highScore->WriteData(s_PlayerName, s_Score, "Gameplay.txt");
		m_IsRunning = false;
		break;
	case Level:
		player->SetPosition(player->GetPosition().x + player->GetVelocity().x * delta * s_SPEED, player->GetPosition().y + player->GetVelocity().y * delta * s_SPEED);

		for (auto& particle : smoke)
		{
			particle->Animate(0, 4, 100, true);
		}

		for (auto& flame : fire)
		{
			flame->Animate(32, 4, 100);
		}

		for (auto& enemy : enemies)
		{
			if (enemy->GetPosition().x < 0 || enemy->GetPosition().x > 1920 - 32 * 3)
			{
				enemy->SetVelocity(enemy->GetVelocity().x * -1, enemy->GetVelocity().y);
			}
			if (enemy->GetPosition().y < 0 || enemy->GetPosition().y > 1080 - 32 * 3)
			{
				enemy->SetVelocity(enemy->GetVelocity().x, enemy->GetVelocity().y * -1);
			}
			enemy->SetPosition(enemy->GetPosition().x + enemy->GetVelocity().x * delta * s_ENEMY_SPEED, enemy->GetPosition().y + enemy->GetVelocity().y * delta * s_ENEMY_SPEED);
		}

		for (int i = 0; i < trees.size(); i++)
		{
			for (auto& enemy : enemies)
			{
				if (enemy->IsColliding(*trees[i]) && !trees[i]->GetIsLit())
				{
					trees[i]->SetIsLit(true);
					smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, trees[i]->GetPosition().x, trees[i]->GetPosition().y - 16, 4.0f, "Smoke")));
					fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 32, trees[i]->GetPosition().x, trees[i]->GetPosition().y - 8, 4.0f, "Fire")));
				}
			}

			for (int j = 0; j < fire.size(); j++)
			{
				if (player->IsColliding(*fire[j]))
				{
					s_Score++;
					fire.erase(fire.begin() + j);
					smoke.erase(smoke.begin() + j);
					for (int k = 0; k < trees.size(); k++)
					{
						trees[k]->SetIsLit(false);
						if (player->IsColliding(*trees[k]))
						{
							trees[k]->SetSrcRect(0, 128);
						}
					}
				}
			}
		}

		for (auto& tree : trees)
		{
			for (auto& helper : helpers)
			{
				if (helper->IsColliding(*tree) && !tree->GetIsLit())
				{
					tree->SetSrcRect(224, 192);
				}
			}
		}

		for (auto& helper : helpers)
		{

			for (int i = 0; i < fire.size(); i++)
			{
				if (helper->IsColliding(*fire[i]))
				{
					fire.erase(fire.begin() + i);
					smoke.erase(smoke.begin() + i);
				}
			}

			for (int i = 0; i < enemies.size(); i++)
			{
				if (helper->IsColliding(*enemies[i]))
				{
					enemies.erase(enemies.begin() + i);
				}
			}

			if (helper->GetPosition().x < 0 || helper->GetPosition().x > 1920 - 32 * 3)
			{
				helper->SetVelocity(helper->GetVelocity().x * -1, helper->GetVelocity().y);
			}
			if (helper->GetPosition().y < 0 || helper->GetPosition().y > 1080 - 32 * 3)
			{
				helper->SetVelocity(helper->GetVelocity().x, helper->GetVelocity().y * -1);
			}
			helper->SetPosition(helper->GetPosition().x + helper->GetVelocity().x * delta * s_HELPER_SPEED, helper->GetPosition().y + helper->GetVelocity().y * delta * s_HELPER_SPEED);
		}

		if (enemies.size() == 0 && fire.size() == 0 && s_Level == 1)
		{
			s_Level = 2;
			tilemap.clear();
			enemies.clear();
			smoke.clear();
			helpers.clear();
			fire.clear();
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
						tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 80, 0, i * 64, j * 64, 4.0f, "GrassTile")));
						break;
					case 1:
						tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 96, 0, i * 64, j * 64, 4.0f, "RoseTile")));
						break;
					case 2:
						tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
						randomTree = rand() % 2;
						if (randomTree == 0)
						{
							trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 240, i * 64, j * 64, 4.0f, "Tree")));
							// smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, i * 64, j * 64 - 24, 4.0f, "Smoke")));
							// fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 32, i * 64, j * 64 - 16, 4.0f, "Fire")));
						}
						else
						{
							trees.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 224, 192, i * 64, j * 64, 4.0f, "Tree")));
							// smoke.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 0, 0, i * 64, j * 64 - 24, 4.0f, "Smoke")));
							// fire.push_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(fireTexture, m_Renderer, 16, 16, 32, 32, i * 64, j * 64 - 16, 4.0f, "Fire")));
						}
						break;
					default:
						tilemap.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(terrainTexture, m_Renderer, 16, 16, 48, 0, i * 64, j * 64, 4.0f, "Tile")));
						break;
					}
				}
			}

			for (int i = 0; i < 12; i++)
			{
				enemies.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(enemyTexture, m_Renderer, 32, 32, 0, 0, 1920 / 2 - 32 * 1.5f, 1080 / 2 - 32 * 1.5f, 3.0f, "Enemy")));
			}

			for (int i = 0; i < 3; i++)
			{
				helpers.emplace_back(std::unique_ptr<Texture2D>(std::make_unique<Texture2D>(helperTexture, m_Renderer, 32, 32, 0, 0, RandomFloat(0, 1920 - 32 * 3), RandomFloat(0, 1080 - 32 * 3), 3.0f, "Helper")));
			}

			for (auto& enemy : enemies)
			{
				enemy->SetVelocity(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
				enemy->SetVelocity(enemy->GetVelocity().Normalize());
			}

			for (auto& helper : helpers)
			{
				helper->SetVelocity(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f));
				helper->SetVelocity(helper->GetVelocity().Normalize());
			}
		}
		if (enemies.size() == 0 && fire.size() == 0 && s_Level == 2)
		{
			s_Scene = Score;
		}

		break;
	case Score:
		std::cout << "Enter your name: " << std::endl;
		std::getline(std::cin, s_PlayerName);
		highScore->WriteData(s_PlayerName, s_Score, "Highscore.txt");
		// highScore->SortData("Highscore.txt");
		highScore->PrintData("Highscore.txt");
		m_IsRunning = false;
		break;
	default:
		break;
	}
}

void Application::OnRender()
{
	SDL_RenderClear(m_Renderer);
	// Render here

	switch (s_Scene)
	{
	case Start:
		startBackground->Render();
		startTitle->Render();
		playButton->Render();
		scoreButton->Render();
		startExitButton->Render();
		break;
	case Pause:
		for (auto& tile : tilemap)
		{
			tile->Render();
		}

		for (auto& tree : trees)
		{
			tree->Render();
		}

		for (auto& enemy : enemies)
		{
			enemy->Render();
		}

		for (auto& particle : smoke)
		{
			particle->Render();
		}

		for (auto& flame : fire)
		{
			flame->Render();
		}
		player->Render();
		pauseOverlay->Render();
		pauseButton->Render();
		resumeButton->Render();
		exitButton->Render();
		break;
	case End:
		break;
	case Level:
		for (auto& tile : tilemap)
		{
			tile->Render();
		}

		for (auto& tree : trees)
		{
			tree->Render();
		}

		for (auto& particle : smoke)
		{
			particle->Render();
		}

		for (auto& flame : fire)
		{
			flame->Render();
		}

		for (auto& enemy : enemies)
		{
			enemy->Render();
		}

		for (auto& helper : helpers)
		{
			helper->Render();
		}

		player->Render();
		break;
	case Score:
		endScoreOverlay->Render();
		endScoreButton->Render();
		break;
	default:
		break;
	}

	cursor->Render();

	SDL_RenderPresent(m_Renderer);
}

bool Application::IsRunning()
{
	return m_IsRunning;
}