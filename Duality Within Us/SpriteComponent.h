#pragma once

#include "Components.h"
#include "TextureManager.h"
#include "SDL.h"
#include "SDL_image.h"

class SpriteComponent : public Component
{
public:
	bool targetted = false;

	SpriteComponent() = default;

	SpriteComponent(const char* path)
	{
		swap(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void swap(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
		crossHair = TextureManager::LoadTexture("Assets/Crosshair/CrossHair.png");
	}

	int getDirection()
	{
		return direction;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		flip = SDL_FLIP_NONE;

		src.x = src.y = 0;
		src.w = transform->width;
		src.h = transform->height;
		dest.w = transform->width * transform->scale;
		dest.h = transform->height * transform->scale;
	}

	void update() override
	{
		dest.x = static_cast<int>(transform->position.x);
		dest.y = static_cast<int>(transform->position.y); 

		if (transform->velocity.x < 0 || transform->velocity.x > 0)
		{
			pv = transform->velocity.x;
		}

		if (pv < 0)
		{
			flip = SDL_FLIP_HORIZONTAL;
			direction = -1;
		}
		else
		{
			flip = SDL_FLIP_NONE;
			direction = 1;
		}
	}

	void setTransparency(int level)
	{
		SDL_SetTextureAlphaMod(texture, level);
	}

	void draw() override
	{
		TextureManager::Draw(texture, src, dest, flip);
		if (targetted)
		{
			TextureManager::Draw(crossHair, src, dest, flip);
		}
	}
private:
	SDL_RendererFlip flip;
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Texture* crossHair;
	SDL_Rect src, dest;
	float pv = 1.0f;
	int direction = 0; // 1 = RIGHT, -1 = LEFT
};