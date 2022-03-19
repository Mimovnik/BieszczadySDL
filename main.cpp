#define _USE_MATH_DEFINES
#include <cmath>
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>

#include <iostream>
#include <vector>

#include "src/Alive.h"
#include "src/Animation.h"
#include "src/Framework.h"
#include "src/GameObject.h"
#include "src/Rectangle.h"
#include "src/RigidBody.h"
#include "src/Terrain.h"
#include "src/Vector.h"
#include "src/control.cpp"
#include "src/displaySetup.cpp"
#include "src/draw.cpp"
#include "src/loadBMP.cpp"
#include "src/settings.h"

int main(int argc, char* args[]) {
    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, "Bieszczady");

    SDL_Surface* screen =
        SDL_CreateRGBSurface(0, display.getWidth(), display.getHeight(), 32,
                             0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Surface *charset = nullptr, *theme = nullptr;

    Vector screenMiddle = Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Vector center = Vector(CENTER_X, CENTER_Y);

    printf("Log:\n");

    std::vector<std::vector<SDL_Surface*>> heroSurfaceListList =
        loadHeroSurfaces();

    int hitboxWidth = 84;
    int hitboxHeigth = 120;
    double walkAcceleration = 20;
    double maxSpeed = 30;
    double jumpHeight = 45;
    double jumpCooldown = 0.5;
    Alive player = Alive(center.add(Vector(1600, -4000)), heroSurfaceListList,
                         hitboxWidth, hitboxHeigth, maxSpeed, walkAcceleration,
                         jumpHeight, jumpCooldown);
    player.rndr.setDrawScaledToHitbox(false);

    std::vector<SDL_Surface*> redSurfaceList;
    redSurfaceList.push_back(loadBMP("../bmp/red.bmp"));

    std::vector<SDL_Surface*> boxSurfaceList;
    boxSurfaceList.push_back(loadBMP("../bmp/box.bmp"));

    GameObject box =
        GameObject(Renderer(boxSurfaceList),
                   RigidBody(Vector::ZERO, BLOCK_WIDTH, BLOCK_HEIGHT));

    // int worldWidth, int worldHeight, double noiseValue, double
    // terrainFreq, double caveFreq, float heightMultiplier, float
    // heightAddition, int dirtLayerHeight, unsigned int seed
    const int worldWidth = 600, worldHeight = 100, blockSize = 64;
    int worldSeed = 1;
    Terrain world =
        Terrain(worldWidth, worldHeight, 0.4, 0.05, 0.08, 25, 25, 5, worldSeed);
    world.generate(charset, screen);
    const int worldSize = worldWidth * worldHeight;

    theme = loadBMP("../bmp/forestTheme3.bmp");

    // char text[128];
    int black = SDL_MapRGB(screen->format, 0, 0, 0);
    int silver = SDL_MapRGB(screen->format, 192, 192, 192);
    int green = SDL_MapRGB(screen->format, 0, 153, 51);
    int red = SDL_MapRGB(screen->format, 204, 0, 0);
    int skyblue = SDL_MapRGB(screen->format, 153, 240, 255);
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

        player.rb.acceleration = gravity;

        // handle input

        std::vector<GameObject> inRangeList =
            world.terrain->queryRange(Rectangle(player.rb.hitbox.width + 100,
                                                player.rb.hitbox.height + 100,
                                                player.rb.hitbox.position));
        int collidersCount = static_cast<int>(inRangeList.size());
        RigidBody* colliders = new RigidBody[collidersCount];
        for (int i = 0; i < collidersCount; i++) {
            colliders[i] = inRangeList[i].rb;
        }

        quit = control(&player, realTime / 1000, colliders, collidersCount, box,
                       world.terrain);

        // change gamestate

        player.rb.collide(colliders, collidersCount, gameDelta);

        player.rb.move(gameDelta, colliders, collidersCount);

        delete[] colliders;

        // output
        SDL_FillRect(screen, NULL, skyblue);

        camera = player.rb.hitbox.position.difference(screenMiddle);

        DrawSurface(screen, theme, center.x, center.y, camera);

        std::vector<GameObject> visibleBlocks = world.terrain->queryRange(
            Rectangle(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300,
                      player.rb.hitbox.position));
        for (int i = 0; i < visibleBlocks.size(); i++) {
            visibleBlocks[i].rndr.draw(screen, camera,
                                       visibleBlocks[i].rb.hitbox);
        }
        player.rndr.draw(screen, camera, player.rb.hitbox);

        // tekst informacyjny
        // DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, silver, brown);

        // sprintf_s(text, "Czas trwania = %.1lf s  %.0lf klatek / s",
        //           realTime / 1000, fps);
        // DrawString(screen,
        //            static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2),
        //            10, text, charset);
        // sprintf_s(text,
        //           "Esc - wyjscie, W / \030 - skok, A / \032 oraz D / \033 - "
        //           "sterowanie");
        // DrawString(screen,
        //            static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2),
        //            26, text, charset);

        display.update(screen);

        // fps counter
        fpsTimer += delta;
        if (fpsTimer > 500) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 500;
        };

        printf(
            "Player's position: x = %.2f y = %.2f "
            "velocity: x = %.8f y = %.8f "
            "acceleration: x = %.4f y = %.4f\n",
            player.rb.hitbox.position.x, player.rb.hitbox.position.y,
            player.rb.velocity.x, player.rb.velocity.y, player.rb.acceleration.x,
            player.rb.acceleration.y);

        frames++;
    }

    // zwolnienie powierzchni
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);

    SDL_Quit();

    return 0;
}