#include "Framework.h"

Display::Display(int width_, int height_, const char* windowTitle)
    : width(width_), height(height_) {
    this->width = width;
    this->height = height;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window,
                                &renderer);
    SDL_SetWindowTitle(window, windowTitle);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    screenTexture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                          SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

Display::~Display() {
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::update(SDL_Surface* surface) {
    // Copy surface to texture
    SDL_DestroyTexture(screenTexture);
    screenTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // Copy texture to renderer
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

Sound::Sound(const char* fileName) {
    if (SDL_LoadWAV(fileName, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cerr << "Audio load error:\n" << SDL_GetError() << std::endl;
    }
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
    if (deviceId == 0) {
        std::cerr << "Open audio device error:\n"
                  << SDL_GetError() << std::endl;
    }
}
Sound::~Sound() {
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}
void Sound::play() {
    status = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    if (status != 0) {
        std::cerr << "Queue audio error:\n" << SDL_GetError() << std::endl;
    }
    SDL_PauseAudioDevice(deviceId, 0);
}