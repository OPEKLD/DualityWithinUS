#include "TutorialManager.h"

TutorialManager::TutorialManager()
{
}

void TutorialManager::init()
{
	for (const char* s : names)
	{
		std::string path = "Assets/TutorialSpecific/" + std::string(s) + ".png";
		textures[std::string(s)] = TextureManager::LoadTexture(path.c_str());
	}
	fontBig = TTF_OpenFont(fontPath, 100);
	fontSmall = TTF_OpenFont(font2Path, 15);
}

void TutorialManager::update()
{
	init();
	WriteText("How To Play", 200, 25, fontBig);

	switch (currentScene)
	{
	case 0:
		DrawTypeAScene(textures["DevilishPrincess"], textures["AngelicHeroine"]);
		break;
	case 1:
		DrawTypeCScene();
		break;
	case 2:
		DrawTypeAScene(textures["AstrocytmareDi"], textures["AstrocytmareZe"]);
		break;
	case 3:
		DrawTypeAScene(textures["CivilianPinkCasual"], textures["InjuredCivilianPink "]);
		break;
	case 4:
		DrawTypeBScene(textures["MedicineGlass"]);
		break;
	case 5:
		DrawTypeBScene(textures["HealthBar"]);
		break;
	}
	destroyTextures();
}

void TutorialManager::nextScene()
{
	currentScene++;
	if (currentScene > 5)
	{
		EndOfTutorial = true;
	}
}

void TutorialManager::destroyTextures()
{
	for (auto& pair : textures)
	{
		SDL_DestroyTexture(pair.second);
	}
	if (fontBig) TTF_CloseFont(fontBig);
	if (fontSmall) TTF_CloseFont(fontSmall);
}

void TutorialManager::WriteText(const char* text, int posX, int posY, TTF_Font* font)
{
	textImage = TextureManager::LoadText(font, white, text);
	int textW, textH;
	SDL_QueryTexture(textImage, nullptr, nullptr, &textW, &textH);
	SDL_Rect textRect = { posX, posY, textW, textH };
	SDL_RenderCopy(Game::renderer, textImage, nullptr, &textRect);
	SDL_DestroyTexture(textImage);
}

void TutorialManager::DrawTypeAScene(SDL_Texture* imageA, SDL_Texture* imageB)
{
	// One hole at 100, one at 500
	//std::cout << "Draw please" << std::endl;
	//SDL_Texture* testing2 = TextureManager::LoadTexture("Assets/TutorialSpecific/HoleInSky.png");
	TextureManager::Draw(textures["HoleInSky"], { 0, 0, 64, 64 }, { 100, 100, 64 * 3, 64 * 3 }, SDL_FLIP_NONE);
	TextureManager::Draw(textures["HoleInSky"], { 0, 0, 64, 64 }, { 500, 100, 64 * 3, 64 * 3 }, SDL_FLIP_NONE);
	TextureManager::Draw(imageA, { 0, 0, 64, 64 }, { 130, 130, 64 * 2, 64 * 2 }, SDL_FLIP_NONE);
	TextureManager::Draw(imageB, { 0, 0, 64, 64 }, { 530, 130, 64 * 2, 64 * 2 }, SDL_FLIP_HORIZONTAL);

	//WriteText(text, 100, 300, fontSmall);
	for (size_t i = 0; i < sceneText[currentScene].size(); i++)
	{
		WriteText(sceneText[currentScene][i], 25, (300 + 30 * i), fontSmall);
	}
}

void TutorialManager::DrawTypeBScene(SDL_Texture* image)
{
	TextureManager::Draw(textures["HoleInSky"], { 0, 0, 64, 64 }, { 100, 100, 64 * 3, 64 * 3 }, SDL_FLIP_NONE);
	TextureManager::Draw(image, { 0, 0, 64, 64 }, { 136, 130, 64 * 2, 64 * 2 }, SDL_FLIP_NONE);

	for (int i = 0; i < 3; i++)
	{
		WriteText(sceneText[currentScene][i], 300, (190 + 30 * i), fontSmall);
	}

	for (int i = 3; i < sceneText[currentScene].size(); i++)
	{
		WriteText(sceneText[currentScene][i], 25, (190 + 30 * i), fontSmall);
	}
}

void TutorialManager::DrawTypeCScene()
{
	TextureManager::Draw(textures["WKey"], { 0, 0, 31, 32 }, { 100, 150, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);
	TextureManager::Draw(textures["SKey"], { 0, 0, 31, 32 }, { 100, 214, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);
	TextureManager::Draw(textures["AKey"], { 0, 0, 31, 32 }, { 38, 214, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);
	TextureManager::Draw(textures["DKey"], { 0, 0, 31, 32 }, { 162, 214, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);

	WriteText(sceneText[currentScene][0], 75, 308, fontSmall);

	TextureManager::Draw(textures["SpaceKey"], { 0, 0, 96, 32 }, { 38, 450, 96 * 2, 32 * 2 }, SDL_FLIP_NONE);

	WriteText(sceneText[currentScene][1], 10, 548, fontSmall);

	TextureManager::Draw(textures["QKey"], { 0, 0, 31, 32 }, { 524, 182, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);

	WriteText(sceneText[currentScene][2], 375, 308, fontSmall);

	TextureManager::Draw(textures["OKey"], { 0, 0, 31, 32 }, { 524, 450, 31 * 2, 32 * 2 }, SDL_FLIP_NONE);

	WriteText(sceneText[currentScene][3], 375, 548, fontSmall);
}
