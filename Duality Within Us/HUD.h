#pragma once

#include <vector>
#include <string>
#include "ECS.h"
#include "Components.h"
#include "TimeManager.h"
#include "TextureManager.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Game.h"

class HUD
{
public:
	HUD(Entity* plr, TimeManager* tm);
	void DrawHealthBar();
	void DrawInyouBar();
	void DrawText();
	const char* GetSprite();
	void render();
private:
	SDL_Rect healthRectdst;
	SDL_Rect conRectdst;
	SDL_Rect hudSrc;
	SDL_Rect headSrc;
	SDL_Rect headDst;
	SDL_Rect inyouRectsrc;
	SDL_Rect inyouDest;

	SDL_Texture* ProgressBar;
	SDL_Texture* PlayerIcon;
	SDL_Texture* Container;
	SDL_Texture* InyouMeter;
	SDL_Texture* Text;

	SDL_Color red = { 255, 0, 0 };

	TTF_Font* font;

	std::vector<std::vector<const char*>> paths;
	Entity* plr;
	TimeManager* tm;
};