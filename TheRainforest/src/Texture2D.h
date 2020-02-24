#pragma once

#include "SDL.h"

class Texture2D
{
public:
	Texture2D();
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcX, int srcY, int destX, int destY, float scale);
	Texture2D(const char* texture, SDL_Renderer* renderer, int srcW, int srcH, int srcX, int srcY, int destX, int destY, float scale);
	virtual ~Texture2D();

	void Update();
	void SetPosition(int x, int y);
	void Render();

private:
	SDL_Texture* m_Texture;
	SDL_Rect m_SrcRect, m_DestRect;
	SDL_Renderer* m_Renderer;
};