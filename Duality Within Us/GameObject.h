#pragma once
#include "Game.h"

class GameObject
{
public:
	GameObject(const char* texturesheet, int width, int height, int xpos, int ypos);
	~GameObject();

	void Update();
	void Render();
private:
	int x, y;
	SDL_Texture* objTex;
	SDL_Rect srcRect, destRect;
};

