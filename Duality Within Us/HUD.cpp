#include "HUD.h"

template<typename T>
T clamp(const T& value, const T& min, const T& max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

HUD::HUD(Entity* plr, TimeManager* tm)
{
	this->plr = plr;
	this->tm = tm;
	hudSrc = { 0, 0, 64, 32 };
	headSrc = { 0, 0, 25, 32 };
	headDst = { -3, 0, 25 * 3, 32 * 3 };
	conRectdst = { 69, 12, hudSrc.w * 3, hudSrc.h * 3 };
	healthRectdst = { 69, 12, 0, 32 * 3 };
	inyouDest = { 198, 12, 0, 32 * 3 };
	inyouRectsrc = {0, 0, 34, 32};

	paths = {
		{"Assets/HUD/Ang1.png", "Assets/HUD/Ang2.png", "Assets/HUD/Ang3.png"},
		{"Assets/HUD/Dev1.png", "Assets/HUD/Dev2.png", "Assets/HUD/Dev3.png"}
	};

	font = TTF_OpenFont("Assets/Font/queens-of-the-apocalypse.ttf", 22);
}

void HUD::DrawHealthBar()
{
	HealthComponent hc = plr->getComponent<HealthComponent>();
	int maxHealth = hc.maxHp;
	int health = hc.hp;

	float healthPercentage = static_cast<float>(health) / maxHealth;
	int baseWidth = 64;
	int scaledWidth = static_cast<int>(baseWidth * 3 * healthPercentage);

	healthRectdst.w = scaledWidth;

	TextureManager::Draw(ProgressBar, hudSrc, healthRectdst, SDL_FLIP_NONE);
}

void HUD::DrawInyouBar()
{
	InyouComponent ic = plr->getComponent<InyouComponent>();
	int aod = ic.angelicordevlish;

	int inyouBaseX = 149;
	float pxlPerUnit = 45.0f / 29.0f;

	int baseWidth = 17;
	int fullWidth = baseWidth * 3;
	if (aod == 0) return;

	float aodPercentage = static_cast<float>(std::abs(aod)) / 30;
	int scaledWidth = static_cast<int>(fullWidth * aodPercentage);

	SDL_RendererFlip flip;

	inyouDest.w = scaledWidth;

	if (aod >= 0)
	{
		InyouMeter = TextureManager::LoadTexture("Assets/HUD/DevilMeter.png");
		inyouDest.x = 198 + Game::camera.x;
		flip = SDL_FLIP_NONE;
	}
	else
	{
		InyouMeter = TextureManager::LoadTexture("Assets/HUD/AngelMeter.png");
		inyouDest.x = (inyouBaseX + static_cast<int>((aod + 30) * pxlPerUnit)) + Game::camera.x;
		flip = SDL_FLIP_HORIZONTAL;
	}

	inyouDest.y = 12 + Game::camera.y;

	TextureManager::Draw(InyouMeter, inyouRectsrc, inyouDest, flip);
}

void HUD::DrawText()
{
	int id = plr->getComponent<InyouComponent>().currentTimer;
	Uint32 remaining = tm->TimeRemaining(id);

	Uint32 seconds = remaining / 1000;
	int minutes = seconds / 60;
	int remainder = seconds % 60;

	std::string minutesTxt = std::to_string(minutes);
	std::string secondsTxt = std::to_string(remainder);
	if (remainder < 10)
	{
		secondsTxt = "0" + secondsTxt;
	}
	std::string time = minutesTxt + ":" + secondsTxt;

	const char* cStr = time.c_str();

	Text = TextureManager::LoadText(font, red, cStr);
	int textW, textH;
	SDL_QueryTexture(Text, nullptr, nullptr, &textW, &textH);
	SDL_Rect textRect = { 110 + Game::camera.x, 58 + Game::camera.y, textW, textH };
	textRect.x = textRect.x - Game::camera.x;
	textRect.y = textRect.y - Game::camera.y;

	SDL_RenderCopy(Game::renderer, Text, nullptr, &textRect);
}

const char* HUD::GetSprite()
{
	InyouComponent ic = plr->getComponent<InyouComponent>();
	HealthComponent hc = plr->getComponent<HealthComponent>();
	int charToUse;

	if (hc.hp > 50) charToUse = 0;
	else if (hc.hp <= 50 && hc.hp > 25) charToUse = 1;
	else charToUse = 2;

	const char* sprite = paths[ic.currentChar][charToUse];
	return sprite;
}

void HUD::render()
{
	ProgressBar = TextureManager::LoadTexture("Assets/HUD/HealthBar.png");
	PlayerIcon = TextureManager::LoadTexture(GetSprite());
	Container = TextureManager::LoadTexture("Assets/HUD/Container.png");
	

	conRectdst.x = healthRectdst.x = 69 + Game::camera.x;
	conRectdst.y = healthRectdst.y = 12 + Game::camera.y;
	headDst.x = -3 + Game::camera.x;
	headDst.y = Game::camera.y;

	DrawHealthBar();
	DrawInyouBar();
	//TextureManager::Draw(InyouMeter, inyouRectsrc, { 147 + Game::camera.x, 12 + Game::camera.y, 34 * 3, 32 * 3 }, SDL_FLIP_NONE);
	TextureManager::Draw(Container, hudSrc, conRectdst, SDL_FLIP_NONE);
	TextureManager::Draw(PlayerIcon, headSrc, headDst, SDL_FLIP_NONE);
	DrawText();



	SDL_DestroyTexture(ProgressBar);
	SDL_DestroyTexture(PlayerIcon);
	SDL_DestroyTexture(Container);
	SDL_DestroyTexture(InyouMeter);
	SDL_DestroyTexture(Text);
}
