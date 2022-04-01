#include "Animation.h"

Animation::Animation(std::vector<SDL_Surface*> leftSurfaceList,
                     std::vector<SDL_Surface*> rightSurfaceList, char* name,
                     double animationSpeed) {
    this->name = name;
    this->leftSurfaceList = leftSurfaceList;
    this->rightSurfaceList = rightSurfaceList;
    this->nextSprite.setCooldown(animationSpeed);
    this->currentSurfaceIndex = 0;
    this->currentSurface = rightSurfaceList[currentSurfaceIndex];
    changeSide('R');
}

void Animation::freeSurfaces() {
    for (int i = 0; i < leftSurfaceList.size(); i++) {
        SDL_FreeSurface(leftSurfaceList[i]);
    }
    leftSurfaceList.shrink_to_fit();
    for (int i = 0; i < rightSurfaceList.size(); i++) {
        SDL_FreeSurface(rightSurfaceList[i]);
    }
    rightSurfaceList.shrink_to_fit();
}

void Animation::changeSurface(double realTime) {
    if (nextSprite.isUp(realTime)) {
        nextSprite.start(realTime);

        currentSurfaceIndex++;
        if (currentSurfaceIndex >= rightSurfaceList.size()) {
            running = false;
        }
        if (side == 'L') {
            currentSurfaceIndex = currentSurfaceIndex % leftSurfaceList.size();
            currentSurface = leftSurfaceList[currentSurfaceIndex];
        }
        if (side == 'R') {
            currentSurfaceIndex = currentSurfaceIndex % rightSurfaceList.size();
            currentSurface = rightSurfaceList[currentSurfaceIndex];
        }
    }
}
void Animation::restart() {
    running = true;
    currentSurfaceIndex = 0;
    this->currentSurface = rightSurfaceList[0];
}

void Animation::stop() {
    running = false;
    currentSurfaceIndex = 0;
    this->currentSurface = rightSurfaceList[0];
}
