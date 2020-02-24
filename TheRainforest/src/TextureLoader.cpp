#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
}

SDL_Texture* TextureLoader::LoadTexture(const char* file, SDL_Renderer* renderer)
{
	SDL_Surface* tmpSurface = IMG_Load(file);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

int TextureLoader::GetTextureHeight(const char* file)
{
	SDL_Surface* tmpSurface = IMG_Load(file);
	int height = tmpSurface->h;
	SDL_FreeSurface(tmpSurface);

	return height;
}

int TextureLoader::GetTextureWidth(const char* file)
{
	SDL_Surface* tmpSurface = IMG_Load(file);
	int width = tmpSurface->w;
	SDL_FreeSurface(tmpSurface);

	return width;
}
