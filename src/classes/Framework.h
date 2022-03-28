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
    Display(int width_, int height_, const char* windowTitle);

    ~Display();

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
    Sound(const char* fileName);

    ~Sound();

    void play();

    void loop() {
        if (SDL_GetQueuedAudioSize(deviceId) <= wavLength) {
            play();
        }
    }

    void stop() { SDL_PauseAudioDevice(deviceId, 1); }
};

#endif