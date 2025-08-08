#pragma once

#include "Game.h"
#include "SDL.h"
#include "SDL_ttf.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static SDL_Texture* LoadText(TTF_Font* font, SDL_Color color, const char* text);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};

