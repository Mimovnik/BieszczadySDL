#include "Framework.h"

void Display::update(SDL_Surface* surface) {
        // Copy surface to texture
        SDL_DestroyTexture(screenTexture);
        screenTexture = SDL_CreateTextureFromSurface(renderer, surface);
        // Copy texture to renderer
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }