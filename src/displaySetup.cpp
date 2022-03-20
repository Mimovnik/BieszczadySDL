#include <SDL.h>

#include <iostream>

#include "settings.h"

void displaySetUp(SDL_Surface** charset, SDL_Surface** screen,
                  SDL_Texture** screenTexture, SDL_Window** window,
                  SDL_Renderer** renderer) {
    char errorMessage[128];
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        sprintf_s(errorMessage, "SDL_Init error: %s\n", SDL_GetError());
        throw errorMessage;
    }

    // tryb pe³noekranowy / fullscreen mode
    int rendererCreated;
    if (FULLSCREEN_ON) {
        rendererCreated = SDL_CreateWindowAndRenderer(
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, window, renderer);
    } else {
        rendererCreated = SDL_CreateWindowAndRenderer(
            SCREEN_WIDTH, SCREEN_HEIGHT, 0, window, renderer);
    }

    if (rendererCreated != 0) {
        SDL_Quit();
        sprintf_s(errorMessage, "SDL_CreateWindowAndRenderer error: %s\n",
                  SDL_GetError());
        throw errorMessage;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);

    SDL_SetWindowTitle(*window, "Bieszczady");

    *screen =
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);

    *screenTexture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       SCREEN_WIDTH, SCREEN_HEIGHT);

    // wczytanie obrazka cs8x8.bmp
    *charset = SDL_LoadBMP("../bmp/cs8x8.bmp");
    if (*charset == NULL) {
        sprintf_s(errorMessage, "SDL_LoadBMP(cs8x8.bmp) error: %s\n",
                  SDL_GetError());
        SDL_FreeSurface(*screen);
        SDL_DestroyTexture(*screenTexture);
        SDL_DestroyWindow(*window);
        SDL_DestroyRenderer(*renderer);
        SDL_Quit();
        throw errorMessage;
    };
    SDL_SetColorKey(*charset, true, 0x000000);
}