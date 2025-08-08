#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int width, int height, int xpos, int ypos)
{
	objTex = TextureManager::LoadTexture(texturesheet);

	x = xpos;
	y = ypos;

	srcRect.w = width;
	srcRect.h = height;
	destRect.w = width;
	destRect.h = height;
}

void GameObject::Update()
{
	x++;

	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = x;
	destRect.y = y;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTex, &srcRect, &destRect);
}
