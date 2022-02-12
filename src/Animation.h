#include <SDL.h>

#include <string>
#include <vector>

#include "Timer.h"

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
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
              double animationSpeed) {
        this->name = name;
        this->leftSurfaceList = leftSurfaceList;
        this->rightSurfaceList = rightSurfaceList;
        this->nextSprite.coolDown = animationSpeed;
        this->currentSurfaceIndex = 0;
        this->currentSurface = rightSurfaceList[0];
        changeSide('R');
    }

    void changeSide(char side) { if(this->side != side) this->side = side; }

    void changeSurface(double realTime) {
        if (nextSprite.isUp(realTime)) {
            nextSprite.start(realTime);

            currentSurfaceIndex++;
            if (side == 'L') {
                currentSurfaceIndex =
                    currentSurfaceIndex % leftSurfaceList.size();
                currentSurface = leftSurfaceList[currentSurfaceIndex];
            }
            if (side == 'R') {
                currentSurfaceIndex =
                    currentSurfaceIndex % rightSurfaceList.size();
                currentSurface = rightSurfaceList[currentSurfaceIndex];
            }
        }
    }
};
#endif