#pragma once

#include "TextureLoader.h"

class Texture2D
{
public:
	Texture2D();
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcX, int srcY, int destX, int destY, float scale);
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcW, int srcH, int srcX, int srcY, int destX, int destY, float scale);
	~Texture2D();

	void Update();
	void Animate(int srcY, int frames, int speed);
	void Animate(int srcY, int frames, int speed, bool flipped);
	void SetPosition(int x, int y);
	void Transform(int dirX, int dirY);
	void Render();

private:
	SDL_Texture* m_Texture;
	SDL_Rect m_SrcRect, m_DestRect;
	SDL_Renderer* m_Renderer;
	SDL_RendererFlip m_Flip;
};