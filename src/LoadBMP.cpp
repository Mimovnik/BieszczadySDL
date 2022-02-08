#pragma once
#include <SDL.h>
#include <iostream>

SDL_Surface* loadBMP(const char* fileName, SDL_Surface* charset,
                     SDL_Surface* screen, SDL_Texture* screenTexture,
                     SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(fileName);
    if (surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(screenTexture);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        throw 1;
    };
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
    return surface;
}