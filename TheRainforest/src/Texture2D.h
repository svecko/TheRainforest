#pragma once

#include "TextureLoader.h"
#include "Vector2D.h"

#include <string>

class Texture2D
{
public:
	Texture2D();
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcX, int srcY, int destX, int destY, float scale);
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcW, int srcH, int srcX, int srcY, int destX, int destY, float scale, const char* entity);
	Texture2D(SDL_Texture* texture, SDL_Renderer* renderer, int srcW, int srcH, int srcX, int srcY, int destX, int destY, float scale, const char* entity);
	Texture2D(const char* texture, SDL_Renderer* renderer);
	~Texture2D();

	void Update();
	void Animate(int srcY, int frames, int speed);
	void Animate(int srcY, int frames, int speed, bool flipped);
	void SetPosition(int x, int y);
	void SetSrcRect(int x, int y);
	Vector2D GetPosition();
	void Transform(int dirX, int dirY);
	void Render();
	bool IsColliding(const Texture2D& other);
	bool IsColliding(int x, int y);
	void SetIsLit(bool value);
	bool GetIsLit();
	std::string GetEntityName();

	int dirX, dirY;

private:
	SDL_Texture* m_Texture;
	SDL_Rect m_SrcRect, m_DestRect;
	SDL_Renderer* m_Renderer;
	SDL_RendererFlip m_Flip;
	bool m_IsLit;
	std::string m_Entity;
};