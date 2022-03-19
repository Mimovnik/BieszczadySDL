
#ifndef LOADBMP_CPP
#define LOADBMP_CPP
#include <SDL.h>

#include <iostream>
#include <vector>
#include "Framework.h"


SDL_Surface* loadBMP(const char* fileName) {
    SDL_Surface* surface = SDL_LoadBMP(fileName);
    if (surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
        throw 1;
    };
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
    return surface;
}

std::vector<std::vector<SDL_Surface*>> loadHeroSurfaces() {

    std::vector<std::vector<SDL_Surface*>> heroSurfaceListList;

    std::vector<SDL_Surface*> heroIdleLeftSurfaceList;
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-00.bmp"));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-01.bmp"));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-02.bmp"));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-03.bmp"));

    std::vector<SDL_Surface*> heroIdleRightSurfaceList;
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-00.bmp"));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-01.bmp"));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-02.bmp"));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-03.bmp"));

    std::vector<SDL_Surface*> heroWalkLeftSurfaceList;
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-00.bmp"));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-01.bmp"));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-02.bmp"));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-03.bmp"));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-04.bmp"));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-05.bmp"));

    std::vector<SDL_Surface*> heroWalkRightSurfaceList;
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-00.bmp"));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-01.bmp"));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-02.bmp"));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-03.bmp"));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-04.bmp"));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-05.bmp"));

    std::vector<SDL_Surface*> heroJumpLeftSurfaceList;
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-00.bmp"));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-01.bmp"));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-02.bmp"));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-03.bmp"));

    std::vector<SDL_Surface*> heroJumpRightSurfaceList;
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-00.bmp"));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-01.bmp"));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-02.bmp"));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-03.bmp"));

    std::vector<SDL_Surface*> heroFallLeftSurfaceList;
    heroFallLeftSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-left-00.bmp"));
    heroFallLeftSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-left-01.bmp"));

    std::vector<SDL_Surface*> heroFallRightSurfaceList;
    heroFallRightSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-right-00.bmp"));
    heroFallRightSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-right-01.bmp"));

    heroSurfaceListList.push_back(heroIdleLeftSurfaceList);
    heroSurfaceListList.push_back(heroIdleRightSurfaceList);
    heroSurfaceListList.push_back(heroWalkLeftSurfaceList);
    heroSurfaceListList.push_back(heroWalkRightSurfaceList);
    heroSurfaceListList.push_back(heroJumpLeftSurfaceList);
    heroSurfaceListList.push_back(heroJumpRightSurfaceList);
    heroSurfaceListList.push_back(heroFallLeftSurfaceList);
    heroSurfaceListList.push_back(heroFallRightSurfaceList);

    return heroSurfaceListList;
}

#endif