#include "Texture2D.h"

#include <memory>

Texture2D::Texture2D()
{
}

Texture2D::Texture2D(const char* texture, SDL_Renderer* renderer, int srcX, int srcY, int destX, int destY, float scale)
	: m_Renderer(renderer)
{
	m_Texture = TextureLoader::LoadTexture(texture, renderer);

	m_SrcRect.h = TextureLoader::GetTextureHeight(texture);
	m_SrcRect.w = TextureLoader::GetTextureWidth(texture);
	m_SrcRect.x = srcX;
	m_SrcRect.y = srcY;

	m_DestRect.h = m_SrcRect.h * scale;
	m_DestRect.w = m_SrcRect.w * scale;
	m_DestRect.x = destX;
	m_DestRect.y = destY;
}

Texture2D::Texture2D(const char* texture, SDL_Renderer* renderer, int srcW, int srcH, int srcX, int srcY, int destX, int destY, float scale)
	: m_Renderer(renderer), m_Flip(SDL_FLIP_NONE)
{
	m_Texture = TextureLoader::LoadTexture(texture, renderer);

	m_SrcRect.h = srcH;
	m_SrcRect.w = srcW;
	m_SrcRect.x = srcX;
	m_SrcRect.y = srcY;

	m_DestRect.h = m_SrcRect.h * scale;
	m_DestRect.w = m_SrcRect.w * scale;
	m_DestRect.x = destX;
	m_DestRect.y = destY;
}

Texture2D::~Texture2D()
{
}

void Texture2D::Update()
{
}

void Texture2D::Animate(int srcY, int frames, int speed)
{
	m_Flip = SDL_FLIP_NONE;
	m_SrcRect.y = srcY;
	m_SrcRect.x = m_SrcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
}

void Texture2D::Animate(int srcY, int frames, int speed, bool flipped)
{
	m_Flip = SDL_FLIP_HORIZONTAL;
	m_SrcRect.y = srcY;
	m_SrcRect.x = m_SrcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
}

void Texture2D::SetPosition(int x, int y)
{
	m_DestRect.x = x;
	m_DestRect.y = y;
}

void Texture2D::Transform(int dirX, int dirY)
{
	m_DestRect.x += dirX;
	m_DestRect.y += dirY;
}

void Texture2D::Render()
{
	SDL_RenderCopyEx(m_Renderer, m_Texture, &m_SrcRect, &m_DestRect, NULL, NULL, m_Flip);
}

