#include <SDL.h>

#include <iostream>
#include <vector>

#ifndef LOADBMP_CPP
#define LOADBMP_CPP

SDL_Surface* loadBMP(const char* fileName, SDL_Surface* charset,
                     SDL_Surface* screen, SDL_Texture* screenTexture,
                     SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(fileName);
    if (surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(screenTexture);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        throw 1;
    };
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
    return surface;
}

std::vector<std::vector<SDL_Surface*>> loadHeroSurfaces(
    SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* screenTexture,
    SDL_Window* window, SDL_Renderer* renderer) {

    std::vector<std::vector<SDL_Surface*>> heroSurfaceListList;

    std::vector<SDL_Surface*> heroIdleLeftSurfaceList;
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleLeftSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-left-03.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroIdleRightSurfaceList;
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroIdleRightSurfaceList.push_back(
        loadBMP("../bmp/player/idle/adventurer-idle-right-03.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroWalkLeftSurfaceList;
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-03.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-04.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkLeftSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-left-05.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroWalkRightSurfaceList;
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-03.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-04.bmp", charset,
                screen, screenTexture, window, renderer));
    heroWalkRightSurfaceList.push_back(
        loadBMP("../bmp/player/walk/adventurer-run-right-05.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroJumpLeftSurfaceList;
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpLeftSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-left-03.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroJumpRightSurfaceList;
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-01.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-02.bmp", charset,
                screen, screenTexture, window, renderer));
    heroJumpRightSurfaceList.push_back(
        loadBMP("../bmp/player/jump/adventurer-jump-right-03.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroFallLeftSurfaceList;
    heroFallLeftSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-left-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroFallLeftSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-left-01.bmp", charset,
                screen, screenTexture, window, renderer));

    std::vector<SDL_Surface*> heroFallRightSurfaceList;
    heroFallRightSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-right-00.bmp", charset,
                screen, screenTexture, window, renderer));
    heroFallRightSurfaceList.push_back(
        loadBMP("../bmp/player/fall/adventurer-fall-right-01.bmp", charset,
                screen, screenTexture, window, renderer));

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