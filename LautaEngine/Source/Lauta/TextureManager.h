#pragma once

#include "Globals.h"
#include <iostream>

class TextureManager
{
public:

	static SDL_Texture* LoadTexture(const char* filename);
	static void Draw(SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_RendererFlip flip, int angle);

};
