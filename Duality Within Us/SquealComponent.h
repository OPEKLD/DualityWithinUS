#pragma once

#include "SDL_mixer.h"
#include "ECS.h"
#include "Components.h"

class SquealComponent : public Component
{
public:
	SquealComponent(Entity* player)
	{
		this->player = player;
	}

	void update() override
	{
		TransformComponent* tc = &entity->getComponent<TransformComponent>();
		TransformComponent* pt = &player->getComponent<TransformComponent>();

		float dx = pt->position.x - tc->position.x;
		float dy = pt->position.y - tc->position.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance < aggro && !isPlaying)
		{
			crying = Mix_LoadMUS("Assets/SoundEffects/help-help-322552.mp3");
			Mix_PlayMusic(crying, -1);
			isPlaying = true;
		}
		else if (distance > aggro && isPlaying)
		{
			Mix_HaltMusic();
			Mix_FreeMusic(crying);
			isPlaying = false;
		}
	}

	void stopAudio()
	{
		Mix_FreeMusic(crying);
		crying = nullptr;
	}
	
private:
	Entity* player;
	Mix_Music* crying;
	bool isPlaying = false;
	float aggro = 300.0f;
};