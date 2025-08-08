#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "ECS.h"
#include "TimeManager.h"

class Game
{
public:
	Game();
	~Game();
	//ProjectileController* pc;
	SDL_Texture* background;
	SDL_Rect dst;

	// Game Manager functions

	void init(const char* title, int x, int y, int w, int h, bool fs);
	void handleEvents();
	void checkCollisions(Entity& subject);
	void update();
	void render();
	void clean();
	void ResetPlayerMovement();
	bool running();
	void displayText(int x, int y, SDL_Texture* text);

	// Game State functions

	void gameMainMenu();
	void gameInit();
	void gameTutorial();
	void gameRunning();
	void transformation();
	void turnOffTimer();
	void EndLevel();
	void GameOver();

	static SDL_Event event;
	static SDL_Renderer* renderer;
	static SDL_Rect camera;
private:
	enum class State{ MAIN_MENU, GAME_INIT, TUTORIAL, RUNNING, TRANSFORMATION, BOSS, END_LEVEL, GAME_OVER};
	State state;
	bool isRunning;
	SDL_Window* window;

	// Transformation State Variables
	SDL_Rect transformFrames[8] =
	{
		{0, 0, 64, 64},
		{64, 0, 64, 64},
		{128, 0, 64, 64},
		{0, 64, 64, 64},
		{64, 64, 64, 64},
		{128, 64, 64, 64},
		{0, 128, 64, 64},
		{64, 128, 64, 64}
	};
	SDL_Color white = { 255, 255, 255 };
	SDL_Color grey = { 100, 100, 100 };
	SDL_Color red = { 255, 0, 0 };
	SDL_Color choice = {100, 100, 100};
	int frameCounter = 0;
	int finalFrame = 7;
	int angle = 0;
	SDL_Texture* transformationSpriteSheet;
	SDL_Texture* disk;
	SDL_Rect trSrc;
	SDL_Rect trDst;
	bool timerActive;

	// End of level variables
	int finalKarma = 0;
	const char* message;
};

