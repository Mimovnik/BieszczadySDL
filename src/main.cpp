#define _USE_MATH_DEFINES
#include <cmath>
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>

#include <iostream>
#include <vector>

#include "Alive.h"
#include "Animation.h"
#include "GameObject.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Terrain.h"
#include "Vector.h"
#include "control.cpp"
#include "displaySetup.cpp"
#include "draw.cpp"
#include "loadBMP.cpp"
#include "settings.h"

int main(int argc, char* args[]) {
    SDL_Surface* screen = nullptr;
    SDL_Texture* screenTexture = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Surface *charset = nullptr, *theme = nullptr;

    Vector screenMiddle = Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Vector center = Vector(CENTER_X, CENTER_Y);

    try {
        displaySetUp(&charset, &screen, &screenTexture, &window, &renderer);
    } catch (char* error) {
        std::cerr << error;
        return 1;
    }

    printf("Log1:\n");

    std::vector<std::vector<SDL_Surface*>> heroSurfaceListList =
        loadHeroSurfaces(charset, screen, screenTexture, window, renderer);

    int hitboxWidth = 84;
    int hitboxHeigth = 120;
    double walkAcceleration = 20;
    double maxSpeed = 30;
    double jumpHeight = 45;
    double jumpCooldown = 0.5;

    Alive player =
        Alive(RigidBody(center.add(Vector(1600, -4000)), hitboxWidth,
                        hitboxHeigth, maxSpeed),
              heroSurfaceListList, walkAcceleration, jumpHeight, jumpCooldown);
    printf("Log3:\n");

    std::vector<SDL_Surface*> redSurfaceList;
    redSurfaceList.push_back(loadBMP("../bmp/red.bmp", charset, screen,
                                     screenTexture, window, renderer));

    std::vector<SDL_Surface*> boxSurfaceList;
    boxSurfaceList.push_back(loadBMP("../bmp/box.bmp", charset, screen,
                                     screenTexture, window, renderer));

    RigidBody boxRb = RigidBody(Vector::ZERO, BLOCK_WIDTH, BLOCK_HEIGHT);
    GameObject box = GameObject(Renderer(boxSurfaceList), &boxRb);

    // int worldWidth, int worldHeight, double noiseValue, double
    // terrainFreq, double caveFreq, float heightMultiplier, float
    // heightAddition, int dirtLayerHeight, unsigned int seed
    const int worldWidth = 600, worldHeight = 100, blockSize = 64;
    int worldSeed = 1;
    Terrain world =
        Terrain(worldWidth, worldHeight, 0.4, 0.05, 0.08, 25, 25, 5, worldSeed);
    world.generate(charset, screen, screenTexture, window, renderer);
    const int worldSize = worldWidth * worldHeight;

    theme = loadBMP("../bmp/forestTheme3.bmp", charset, screen, screenTexture,
                    window, renderer);

    char text[128];
    int black = SDL_MapRGB(screen->format, 0, 0, 0);
    int silver = SDL_MapRGB(screen->format, 192, 192, 192);
    int green = SDL_MapRGB(screen->format, 0, 153, 51);
    int red = SDL_MapRGB(screen->format, 204, 0, 0);
    int blue = SDL_MapRGB(screen->format, 153, 240, 255);
    int brown = SDL_MapRGB(screen->format, 102, 51, 0);

    int lastTime = 0, frames = 0;
    int currentTime, delta;
    double fpsTimer = 0, fps = 0, realTime = 0, gameTime = 0, timeFactor = 0.01;
    double gameDelta;
    bool quit = false;
    lastTime = SDL_GetTicks();
    Vector camera;

    Vector gravity = Vector(0, 10);

    while (!quit) {
        currentTime = SDL_GetTicks();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        gameDelta = delta * timeFactor;
        if (delta < (1000 / MAX_FPS)) SDL_Delay(1000 / MAX_FPS - delta);

        realTime += delta;
        gameTime += gameDelta;

        SDL_FillRect(screen, NULL, blue);

        camera = player.rb->hitbox.position.difference(screenMiddle);

        DrawSurface(screen, theme, center.x, center.y, camera);

        player.rb->acceleration = gravity;
        // player.acceleration = Vector::ZERO;

        std::vector<GameObject> inRangeList =
            world.terrain->queryRange(Rectangle(player.rb->hitbox.width + 100,
                                                player.rb->hitbox.height + 100,
                                                player.rb->hitbox.position));
        int blocksInRangeCount = static_cast<int>(inRangeList.size());
        GameObject* blockInRange = new GameObject[blocksInRangeCount];
        std::copy(inRangeList.begin(), inRangeList.end(), blockInRange);
        RigidBody* rbsInRange = new RigidBody[blocksInRangeCount];
        for (int i = 0; i < blocksInRangeCount; i++) {
            rbsInRange[i] = *blockInRange[i].rb;
        }

        quit = control(&player, realTime / 1000, rbsInRange, blocksInRangeCount,
                       box, world.terrain);
        // quit = noclip(&player);

        player.rb->collide(rbsInRange, blocksInRangeCount, gameDelta);

        std::vector<GameObject> visibleBlocks = world.terrain->queryRange(
            Rectangle(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300,
                      player.rb->hitbox.position));
        for (int i = 0; i < visibleBlocks.size(); i++) {
            visibleBlocks[i].renderer.draw(screen, visibleBlocks[i].position(),
                                           camera);
        }
        player.renderer.draw(screen, player.position(), camera);

        fpsTimer += delta;
        if (fpsTimer > 500) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 500;
        };

        // tekst informacyjny
        DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, silver, brown);

        sprintf_s(text, "Czas trwania = %.1lf s  %.0lf klatek / s",
                  realTime / 1000, fps);
        DrawString(screen,
                   static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2), 10,
                   text, charset);
        //	      "Esc - exit, \030 - faster, \031 - slower"
        sprintf_s(text,
                  "Esc - wyjscie, W / \030 - skok, A / \032 oraz D / \033 - "
                  "sterowanie");
        DrawString(screen,
                   static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2), 26,
                   text, charset);

        SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch);

        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        player.rb->move(gameDelta, rbsInRange, blocksInRangeCount);

        delete[] rbsInRange;

        printf(
            "Player's position: x = %.2f y = %.2f "
            "velocity: x = %.8f y = %.8f "
            "acceleration: x = %.4f y = %.4f\n",
            player.rb->hitbox.position.x, player.rb->hitbox.position.y,
            player.rb->velocity.x, player.rb->velocity.y,
            player.rb->acceleration.x, player.rb->acceleration.y);

        frames++;
    }

    // zwolnienie powierzchni
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}