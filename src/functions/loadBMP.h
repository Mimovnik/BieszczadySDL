
#ifndef LOADBMP_H
#define LOADBMP_H
#include <SDL.h>

#include <iostream>
#include <string>
#include <vector>

SDL_Surface* loadBMP(std::string fileName);

SDL_Surface* createFlipV(SDL_Surface* surface);

SDL_Surface* createFlipH(SDL_Surface* surface);

std::vector<std::vector<SDL_Surface*>> loadSurfaces(
    std::string folder, const int idleCount, const int walkCount,
    const int jumpCount, const int fallCount, const int attack1Count, const int attackUpCount, const int attackDownCount,
    const int hurtCount, const int dieCount, bool idleIsFly);

#endif