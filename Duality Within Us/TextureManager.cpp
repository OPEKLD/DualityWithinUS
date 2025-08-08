#include "TextureManager.h"

SDL_Texture* TextureManager::LoadText(TTF_Font* font, SDL_Color color, const char* text)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    dest.x = dest.x - Game::camera.x;
    dest.y = dest.y - Game::camera.y;
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0.0, nullptr, flip);
}
