#include "Renderer.h"

void Renderer::draw(SDL_Surface* screen, Vector position, Vector offset) {
    SDL_Rect dest;
    if (active != nullptr) {
        dest.x = position.x - active->currentSurface->w / 2 - offset.x;
        dest.y = position.y - active->currentSurface->h / 2 - offset.y;
        dest.w = active->currentSurface->w;
        dest.h = active->currentSurface->h;
        SDL_BlitScaled(active->currentSurface, NULL, screen, &dest);

    } else {
        dest.x = position.x - surface->w / 2 - offset.x;
        dest.y = position.y - surface->h / 2 - offset.y;
        dest.w = surface->w;
        dest.h = surface->h;
        SDL_BlitScaled(surface, NULL, screen, &dest);
    }
}

