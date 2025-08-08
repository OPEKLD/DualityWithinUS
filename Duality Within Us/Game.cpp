#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Collisions.h"
#include "Vector2D.h"
#include "ProjectileController.h"
#include "EnemySpawner.h"
#include "CivilianSpawner.h"
#include "MedPackSpawner.h"
#include "HUD.h"
#include "TutorialManager.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

// Set up object variables
Map* map;
Collision* collisionManager;
TimeManager* tm;
TutorialManager* tum;
ProjectileController* pc;
EnemySpawner* es;
CivilianSpawner* cs;
MedPackSpawner* mps;
HUD* hud;
TTF_Font* font;
TTF_Font* endingFont;

// Set up game renderer, event, and camera
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0, 0, 800, 640 };

// Set up manager and player
Manager manager;
auto& playerEntity(manager.addEntity());

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int x, int y, int w, int h, bool fs)
{
	int flags = 0;
	if (fs)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window)
		{
			std::cout << "Window creation successful" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			std::cout << "Renderer creation succesful" << std::endl;
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	TTF_Init();

	tum = new TutorialManager();
	tum->init();
	//state = State::GAME_INIT;
	state = State::MAIN_MENU;
	trDst = { 0, 0, 800, 640 };
	font = TTF_OpenFont("Assets/Font/queens-of-the-apocalypse.ttf", 100);
	endingFont = TTF_OpenFont("Assets/Font/queens-of-the-apocalypse.ttf", 40);
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			if (state == State::TUTORIAL)
			{
				tum->nextScene();
				if (tum->EndOfTutorial == true)
				{
					state = State::GAME_INIT;
				}
			}
			
		}
		else if (event.key.keysym.sym == SDLK_RETURN && state == State::MAIN_MENU)
		{
			state = State::TUTORIAL;
		}
		
	}
}

void Game::update()
{
	switch (state)
	{
	case State::MAIN_MENU:
		gameMainMenu();
		break;
	case State::GAME_INIT:
		gameInit();
		break;
	case State::TUTORIAL:
		gameTutorial();
		break;
	case State::RUNNING:
		gameRunning();
		break;
	case State::TRANSFORMATION:
		transformation();
		break;
	case State::BOSS:
		break;
	case State::GAME_OVER:
		GameOver();
		break;
	case State::END_LEVEL:
		EndLevel();
		break;
	}

	
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	if (state == State::GAME_INIT || state == State::RUNNING)
	{
		TextureManager::Draw(background, { 0, 0, 800, 640 }, dst, SDL_FLIP_NONE);
		map->DrawMap();
		manager.draw();
		hud->render();
	}
	else if (state == State::TRANSFORMATION)
	{
		TextureManager::Draw(background, { 0, 0, 800, 640 }, dst, SDL_FLIP_NONE);
		TextureManager::Draw(transformationSpriteSheet, trSrc, trDst, SDL_FLIP_NONE);
	}
	else if (state == State::TUTORIAL)
	{
		tum->update();
	}
	else if (state == State::MAIN_MENU)
	{
		SDL_Rect srcRect = { 0, 0, 128, 128 };
		SDL_Rect dstRect = { 260, 190, 256, 256 };
		SDL_Point center = { dstRect.w / 2, dstRect.h / 2 };
		SDL_RenderCopyEx(renderer, disk, &srcRect, &dstRect, angle, &center, SDL_FLIP_NONE);
		SDL_DestroyTexture(disk);

		SDL_Texture* title = TextureManager::LoadText(font, white, "Duality Within Us");
		SDL_Texture* prompt = TextureManager::LoadText(font, white, "Press Enter");

		displayText(125, 50, title);
		displayText(200, 500, prompt);

		SDL_DestroyTexture(title);
		SDL_DestroyTexture(prompt);
	}
	else if (state == State::END_LEVEL)
	{
		SDL_Texture* endingMessage = TextureManager::LoadText(endingFont, choice, message);

		displayText(50, 300, endingMessage);

		SDL_DestroyTexture(endingMessage);

	}
	else if (state == State::GAME_OVER)
	{
		SDL_Texture* gameOver = TextureManager::LoadText(font, red, "GAME OVER");

		displayText(200, 300, gameOver);

		SDL_DestroyTexture(gameOver);
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::ResetPlayerMovement()
{
	TransformComponent* tc = &playerEntity.getComponent<TransformComponent>();
	if (tc->velocity.x > 0)
	{
		tc->position.x -= 5;
	}
	else if (tc->velocity.x < 0)
	{
		tc->position.x += 5;
	}

	if (tc->velocity.y > 0)
	{
		tc->position.y -= 5;
	}
	else if (tc->velocity.y < 0)
	{
		tc->position.y += 5;
	}

	tc->velocity.x = 0;
	tc->velocity.y = 0;
}

bool Game::running()
{
	return isRunning;
}

void Game::displayText(int x, int y, SDL_Texture* text)
{
	int textW, textH;
	SDL_QueryTexture(text, nullptr, nullptr, &textW, &textH);
	SDL_Rect textRect = { x, y, textW, textH };
	SDL_RenderCopy(renderer, text, nullptr, &textRect);
}

// GAME STATE FUNCTIONS

void Game::gameMainMenu()
{
	angle++;
	disk = TextureManager::LoadTexture("Assets/TitleScreen/Disk.png");
}

void Game::gameInit()
{
	// Initialize background
	background = TextureManager::LoadTexture("Assets/Backgrounds/Morning.png");
	transformationSpriteSheet = TextureManager::LoadTexture("Assets/Transformation/AngelToDevil.png");
	dst = { 0, 0, 800, 640 };

	// Add Map, Collision Manager, and Time Manager classes
	map = new Map();
	tm = new TimeManager();
	collisionManager = new Collision(&manager, tm);

	// Initialize Player's components
	playerEntity.addComponent<TransformComponent>(0, 0, 39, 48, 2);
	playerEntity.addComponent<SpriteComponent>("Assets/Player/AngelicHeroine.png");
	playerEntity.addComponent<InyouComponent>(tm);
	playerEntity.addComponent<KeyboardController>();
	playerEntity.addComponent<HealthComponent>(100, 100, tm);
	playerEntity.addComponent<ColliderComponent>("player");
	playerEntity.getComponent<InyouComponent>().startProcessing();

	// Initialize Projectile Controller, Enemy Spawner, and Civilian Spawner
	pc = new ProjectileController(&manager, &playerEntity, tm);
	es = new EnemySpawner(&manager, &playerEntity, collisionManager, tm);
	cs = new CivilianSpawner(&manager, collisionManager, tm, &playerEntity);
	mps = new MedPackSpawner(&manager, collisionManager);
	es->SpawnEnemies();
	cs->spawn();
	mps->spawn();

	// Create HUD
	hud = new HUD(&playerEntity, tm);

	state = State::RUNNING;
}

void Game::gameTutorial()
{
	if (tum->EndOfTutorial == true)
	{
		state = State::GAME_INIT;
	}
}

void Game::gameRunning()
{
	// Check if Player is hitting TileMap and stop them from moving further
	bool hittingTile = map->CheckMapCollisions(&playerEntity);
	if (hittingTile)
	{
		ResetPlayerMovement();
	}
	
	// Update Entities Components
	manager.refresh();
	manager.update();

	// Update Projectile controller, Enemy spawner, and Civilian Spawner
	pc->update();
	tm->update();
	es->EnemyUpdate();
	cs->update();
	mps->update();

	// Check collisions for player and medpack
	collisionManager->checkCollisions(playerEntity);

	// Move and clamp camera
	Game::camera.x = static_cast<int>(playerEntity.getComponent<TransformComponent>().position.x + playerEntity.getComponent<TransformComponent>().width / 2 - Game::camera.w / 2);
	Game::camera.y = static_cast<int>(playerEntity.getComponent<TransformComponent>().position.y + playerEntity.getComponent<TransformComponent>().height / 2 - Game::camera.h / 2);

	if (Game::camera.x < 0) Game::camera.x = 0;
	if (Game::camera.x > 4650) Game::camera.x = 4650;
	if (Game::camera.y < 0) Game::camera.y = 0;
	if (Game::camera.y > 320) Game::camera.y = 320;

	// Make Background follow player
	dst.x = Game::camera.x;
	dst.y = Game::camera.y;
	trDst.x = Game::camera.x;
	trDst.y = Game::camera.y;

	if (playerEntity.getComponent<InyouComponent>().timerTimeout == true)
	{
		state = State::TRANSFORMATION;
	}

	if (playerEntity.getComponent<HealthComponent>().hp <= 0)
	{
		state = State::GAME_OVER;
	}

	if (es->gameFinished == true)
	{
		state = State::END_LEVEL;
	}
}

void Game::transformation()
{
	tm->update();

	trSrc = transformFrames[frameCounter];

	if (!timerActive)
	{
		Uint32 time = (frameCounter == 0 || frameCounter == 7) ? 2000 : 100;
		
		tm->AddTimer(time, [this]()
		{
			this->turnOffTimer();
		});
		timerActive = true;
	}
	TextureManager::Draw(transformationSpriteSheet, trSrc, trDst, SDL_FLIP_NONE);
}

void Game::turnOffTimer()
{
	timerActive = false;

	if (frameCounter == finalFrame)
	{
		state = State::RUNNING;
		playerEntity.getComponent<InyouComponent>().switchCharacter();
		if (frameCounter == 7)
		{
			finalFrame = 0;
		}
		else
		{
			finalFrame = 7;
		}
	}

	if (playerEntity.getComponent<InyouComponent>().currentChar == 0)
	{
		frameCounter++;
	}
	else
	{
		frameCounter--;
	}
}

void Game::EndLevel()
{
	finalKarma = playerEntity.getComponent<InyouComponent>().angelicordevlish;

	if (finalKarma <= -25)
	{
		message = "Your heart has an angelic tune, your care is bright";
		choice = white;
	}
	else if (finalKarma > -25 && finalKarma <= -10)
	{
		message = "You have some good buried deep deep within you.";
		choice = white;
	}
	else if (finalKarma >= -9 && finalKarma <= 9)
	{
		message = "Your darkness and brightness contrast well inside you";
		choice = grey;
	}
	else if (finalKarma >= 10 && finalKarma < 25)
	{
		message = "You have some evil buried deep within you. It can't hide";
		choice = red;
	}
	else if (finalKarma >= 25)
	{
		message = "Your heart bears a deep darkness and despair.";
		choice = red;
	}
}

void Game::GameOver()
{
	// Destroy and reinitialize all entities
}
