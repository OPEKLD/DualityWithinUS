#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "TextureManager.h"
#include "Game.h"
#include <unordered_map>

class TutorialManager
{
public:
	
	bool EndOfTutorial = false;

	TutorialManager();

	void init();
	void update();
	void nextScene();
	void destroyTextures();

	void WriteText(const char* text, int posX, int posY, TTF_Font* font);

	void DrawTypeAScene(SDL_Texture* imageA, SDL_Texture* imageB);
	void DrawTypeBScene(SDL_Texture* image);
	void DrawTypeCScene();

private:
	int currentScene = 0;

	// Textures 
	SDL_Texture* test;
	std::vector<const char *> names = {
		"AKey",
		"AngelicHeroine",
		"DevilishPrincess",
		"DKey",
		"HoleInSky",
		"OKey",
		"QKey",
		"SKey",
		"SpaceKey",
		"WKey",
		"AstrocytmareDi",
		"AstrocytmareZe",
		"CivilianPinkCasual",
		"InjuredCivilianPink ",
		"MedicineGlass",
		"HealthBar"
	};
	std::unordered_map<std::string, SDL_Texture*> textures;

	// Text
	SDL_Color white = { 255, 255, 255 };
	const char* fontPath = "Assets/Font/queens-of-the-apocalypse.ttf";
	const char* font2Path = "Assets/Font/press-start.regular.ttf";
	TTF_Font* fontBig;
	TTF_Font* fontSmall;
	SDL_Texture* textImage;

	std::vector<std::vector<const char*>> sceneText =
	{
		{
			"In this game you will be taking control of two",
			"different characters, A Devilish Princess and ",
			"an Angelic Heroine. Both characters will have ",
			"the ability to shoot projectiles, but the ",
			"Devilish Princess will also have a homing attack.",
			"The homing attack doesn't just target enemies, but",
			"also innocents."
		},
		{
			"Movement",
			"Shoot Projectiles",
			"Break Medpack (if holding)",
			"Homing Attack (Devil only)"
		},
		{
			"The \"Goal\" of the level is to defeat all ",
			"enemies. The level will be divided into three",
			"waves, each slightly more challenging."
		},
		{
			"On the ground level, there will be innocent ",
			"civilians. These are innocent creatures that",
			"inhabit the land the monsters are invading. ",
			"Some are injured and need help...",
			"their screams are annoying...",
			"maybe I should shoot th... ANYWAYS"
		},
		{
			"There are medpacks scattered",
			"across the map. They can be",
			"brought to civilians to save",
			"them, something a truly good-hearted individual",
			"would do. Medpacks can also be broken by",
			"pressing Q to restore health, but that's a bit",
			"selfish and cruel to the ones suffering below,",
			"isn't it?"
		},
		{
			"Well, that about covers it.",
			"The rest is standard stuff.",
			"You have your health bar, ",
			"lives, and timer to show when you will switch ",
			"characters at the top left... What's that?... ",
			"That bar under the health? Don't worry about that",
			"... Just have fun and be yourself!",
			"Oh btw, giving injured civilians health packs ",
			"isn't the only way to stop their screaming, it's",
			"just the nice one... Hope you remember the ",
			"controls!"
		}
	};
};