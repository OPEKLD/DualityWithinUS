#define SDL_MAIN_HANDLED
#include "Game.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif 


Game* game = nullptr;

const char* setTitle()
{
	const char* returnValue = "Locale Not Found";
#ifdef _WIN32
	auto var = GetThreadLocale();
	switch (var)
	{
	case 1041:
		returnValue = "心陰陽";
		break;
	case 3072:
		returnValue = "Duality within Us";
		break;
	}
#else
	const char* lang = std::getenv("LANG");
	if (lang != nullptr)
	{
		if (lang == "ja")
		{
			returnValue = "心陰陽";
		}
		else if (lang == "en")
		{
			returnValue = "Duality within Us";
		}
	}
#endif
	return returnValue;
}

int main()
{
	game = new Game();
	const char* title = setTitle();
	game->init(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}