#ifndef ANIMATION_H
#define ANIMATION_H
#include <SDL.h>

#include <string>
#include <vector>

#include "Timer.h"

class Animation {
    bool running = false;

   public:
    std::string name;
    std::vector<SDL_Surface*> rightSurfaceList;
    std::vector<SDL_Surface*> leftSurfaceList;
    char side;
    SDL_Surface* currentSurface = nullptr;
    int currentSurfaceIndex;
    Timer nextSprite;

    Animation(){};

    Animation(std::vector<SDL_Surface*> leftSurfaceList,
              std::vector<SDL_Surface*> rightSurfaceList, char* name,
              double animationSpeed);

    void changeSide(char side) { this->side = side; }

    void changeSurface(double realTime);

    void restart();

    void stop();

    bool isRunning() { return running; }
};
#endif