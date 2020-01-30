#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) 
{
	SDL_Surface* tmpSurface = IMG_Load(texture);
	if (tmpSurface == NULL) 
	{
		printf("Unable to load image %s! SDL_Image Error: %s\n", texture, IMG_GetError());
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(Globals::Global_Renderer, tmpSurface);
	if (tex == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", texture, SDL_GetError());
	}

	SDL_FreeSurface(tmpSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip, int angle)
{
	SDL_RenderCopyEx(Globals::Global_Renderer, texture, &sourceRect, &destinationRect, angle, NULL, flip);
}
