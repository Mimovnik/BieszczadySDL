#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <SDL.h>

#include <iostream>
class Display {
    int width;
    int height;
    SDL_Window* window = nullptr;
    SDL_Texture* screenTexture = nullptr;
    SDL_Renderer* renderer = nullptr;

   public:
    Display(int width_, int height_, const char* windowTitle)
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

    ~Display() {
        SDL_DestroyTexture(screenTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void update(SDL_Surface* surface);

    SDL_Surface* createScreen() {
        return SDL_CreateRGBSurface(0, getWidth(), getHeight(), 32, 0x00FF0000,
                                    0x0000FF00, 0x000000FF, 0xFF000000);
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
};

class Sound {
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;
    SDL_AudioDeviceID deviceId;
    int status;

   public:
    Sound(const char* fileName) {
        if (SDL_LoadWAV(fileName, &wavSpec, &wavBuffer, &wavLength) ==
            nullptr) {
            std::cerr << "Audio load error:\n" << SDL_GetError() << std::endl;
        }
        deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
        if (deviceId == 0) {
            std::cerr << "Open audio device error:\n"
                      << SDL_GetError() << std::endl;
        }
    }
    ~Sound() {
        SDL_CloseAudioDevice(deviceId);
        SDL_FreeWAV(wavBuffer);
    }
    void play() {
        status = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
        if (status != 0) {
            std::cerr << "Queue audio error:\n" << SDL_GetError() << std::endl;
        }
        SDL_PauseAudioDevice(deviceId, 0);
    }

    void loop() {
        if (SDL_GetQueuedAudioSize(deviceId) <= wavLength) {
            play();
        }
    }

    void stop() { SDL_PauseAudioDevice(deviceId, 1); }
};

#endif