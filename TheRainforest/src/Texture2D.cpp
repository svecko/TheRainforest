#include "Texture2D.h"
#include "TextureLoader.h"

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
	: m_Renderer(renderer)
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

void Texture2D::SetPosition(int x, int y)
{
	m_DestRect.x = x;
	m_DestRect.y = y;
}

void Texture2D::Render()
{
	SDL_RenderCopy(m_Renderer, m_Texture, &m_SrcRect, &m_DestRect);
	// SDL_RenderCopyEx(m_Renderer, m_Texture, &m_SrcRect, &m_DestRect, m_Angle, NULL, SDL_FLIP_NONE);
}

