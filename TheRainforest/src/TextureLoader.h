#pragma once

#include <SDL.h>
#include <SDL_image.h>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	static SDL_Texture* LoadTexture(const char* file, SDL_Renderer* renderer);
	static int GetTextureHeight(const char* file);
	static int GetTextureWidth(const char* file);
};
