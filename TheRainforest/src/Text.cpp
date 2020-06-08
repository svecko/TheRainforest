#include "Text.h"

Text::Text()
{
}

Text::Text(SDL_Renderer* renderer, int x, int y, std::string text, SDL_Color textColor, const char* path, int fontSize)
{
	m_FontSize = fontSize;
	m_Font = TTF_OpenFont(path, m_FontSize);

	m_Rect.x = x;
	m_Rect.y = y;

	m_Renderer = renderer;

	SDL_Surface* tmpSurface = TTF_RenderText_Blended(m_Font, text.c_str(), textColor);
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_QueryTexture(m_Texture, nullptr, nullptr, &m_Rect.w, &m_Rect.h);
}

void Text::Render(int x, int y, std::string text, SDL_Color textColor)
{
	m_Rect.x = x;
	m_Rect.y = y;

	SDL_Surface* tmpSurface = TTF_RenderText_Blended(m_Font, text.c_str(), textColor);
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_QueryTexture(m_Texture, nullptr, nullptr, &m_Rect.w, &m_Rect.h);
	SDL_RenderCopy(m_Renderer, m_Texture, nullptr, &m_Rect);
	SDL_DestroyTexture(m_Texture);
}

Text::~Text()
{
}
