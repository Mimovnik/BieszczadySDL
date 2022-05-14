#ifndef SURFACES_H
#define SURFACES_H

#include <vector>

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class Surfaces {
    SDL_Surface* youdied = nullptr;
    SDL_Surface* charset = nullptr;
    SDL_Surface* healthPoint = nullptr;
    SDL_Surface* cursor = nullptr;
    std::vector<SDL_Surface*> boxSurfaces;
    std::vector<std::vector<SDL_Surface*>> playerAnimations;
    std::vector<std::vector<SDL_Surface*>> mobAnimations;

   public:
    void load();

    ~Surfaces();

    SDL_Surface* getYoudied() const;
    SDL_Surface* getCharset() const;
    SDL_Surface* getHealthPoint() const;
    SDL_Surface* getCursor() const;
    std::vector<SDL_Surface*> getBoxSurfaces() const;
    std::vector<std::vector<SDL_Surface*>> getPlayerAnimations() const;
    std::vector<std::vector<SDL_Surface*>> getMobAnimations() const;
};

#endif