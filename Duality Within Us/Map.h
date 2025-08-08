#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class Map
{
public:
	Map();
	~Map();

	void LoadMap(int arr[20][115]);
	void DrawMap();
	bool CheckMapCollisions(Entity* player);

private:

	SDL_Rect src, dest;
	SDL_Texture* TestingGrassTiles;

	int map[20][115];
	int tilesetWidth = 48;
	const int TILE_SIZE = 16;
};

