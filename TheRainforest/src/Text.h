#pragma once

#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <SDL.h>

class Text
{
public:
	Text();
	Text(SDL_Renderer* renderer, int x, int y, std::string text, SDL_Color textColor, const char* path, int fontSize);

	void Render(int x, int y, std::string text, SDL_Color textColor);
	~Text();

private:
	SDL_Texture* m_Texture;
	SDL_Rect m_Rect;
	SDL_Renderer* m_Renderer;
	int m_FontSize;
	TTF_Font* m_Font;
};