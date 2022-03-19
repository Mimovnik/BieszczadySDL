#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include "Animation.h"
#include "Vector.h"
#include "Rectangle.h"

class Renderer {
    SDL_Surface* surface = nullptr;
    bool drawScaledToHitbox;

   public:
    Animation* active = nullptr;
    Renderer();

    Renderer(std::vector<SDL_Surface*> idleSurfaceList,
             bool drawScaledToHitbox = true);
    ~Renderer();

    void draw(SDL_Surface* screen, Vector offset, Rectangle hitbox);

    void setDrawScaledToHitbox(bool dsth);
};

#endif