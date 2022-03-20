#include "Renderer.h"

Renderer::Renderer() {}

Renderer::Renderer(std::vector<SDL_Surface*> idleSurfaceList,
                   bool drawScaledToHitbox) {
    this->drawScaledToHitbox = drawScaledToHitbox;
    this->surface = idleSurfaceList[0];
}

Renderer::~Renderer() {}

void Renderer::setDrawScaledToHitbox(bool dsth) {
    this->drawScaledToHitbox = dsth;
}

void Renderer::draw(SDL_Surface* screen, Vector offset, Rectangle hitbox) {
    SDL_Rect dest;
    if (active != nullptr) {
        if (drawScaledToHitbox) {
            dest.x = static_cast<int>(hitbox.position.x - hitbox.width / 2 -
                                      offset.x);
            dest.y = static_cast<int>(hitbox.position.y - hitbox.height / 2 -
                                      offset.y);
            dest.w = static_cast<int>(hitbox.width);
            dest.h = static_cast<int>(hitbox.height);
            SDL_BlitScaled(active->currentSurface, NULL, screen, &dest);
        } else {
            dest.x = static_cast<int>(hitbox.position.x -
                                      active->currentSurface->w / 2 - offset.x);
            dest.y = static_cast<int>(hitbox.position.y -
                                      active->currentSurface->h / 2 - offset.y);
            dest.w = active->currentSurface->w;
            dest.h = active->currentSurface->h;
            SDL_BlitScaled(active->currentSurface, NULL, screen, &dest);
        }
    } else {
        if (drawScaledToHitbox) {
            dest.x = static_cast<int>(hitbox.position.x - hitbox.width / 2 -
                                      offset.x);
            dest.y = static_cast<int>(hitbox.position.y - hitbox.height / 2 -
                                      offset.y);
            dest.w = static_cast<int>(hitbox.width);
            dest.h = static_cast<int>(hitbox.height);
            SDL_BlitScaled(surface, NULL, screen, &dest);
        } else {
            dest.x =
                static_cast<int>(hitbox.position.x - surface->w / 2 - offset.x);
            dest.y =
                static_cast<int>(hitbox.position.y - surface->h / 2 - offset.y);
            dest.w = surface->w;
            dest.h = surface->h;
            SDL_BlitScaled(surface, NULL, screen, &dest);
        }
    }
}