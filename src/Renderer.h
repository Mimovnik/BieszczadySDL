#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

#include <vector>

#include "Animation.h"
#include "Vector.h"

class Renderer {
   public:
    Animation* active = nullptr;
    SDL_Surface* surface = nullptr;

    Renderer() {}

    Renderer(std::vector<SDL_Surface*> idleSurfaceList) {
        this->surface = idleSurfaceList[0];
    }

    void draw(SDL_Surface* screen, Vector position, Vector offset);

    void setActive(Animation* active) { this->active = active; }


    ~Renderer() {
        if (active != nullptr) {
            delete active;
        }

        if (surface != nullptr) {
            delete surface;
        }
    }
};

#endif