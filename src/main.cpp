﻿#define _USE_MATH_DEFINES
#include <cmath>
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>

#include <iostream>
#include <vector>

#include "classes/Alive.h"
#include "classes/Animation.h"
#include "classes/Framework.h"
#include "classes/GameObject.h"
#include "classes/Rectangle.h"
#include "classes/RigidBody.h"
#include "classes/Terrain.h"
#include "classes/Vector.h"
#include "functions/controller.h"
#include "functions/draw.h"
#include "functions/loadBMP.h"
#include "functions/settings.h"

void calculateNearbyColliders(RigidBody* rb, QuadTree* terrain) {
    if (rb->colliders != nullptr) {
        delete[] rb->colliders;
        rb->collidersCount = 0;
    }
    std::vector<GameObject> inRangeList = terrain->queryRange(Rectangle(
        rb->hitbox.width + 200, rb->hitbox.height + 200, rb->hitbox.position));
    rb->collidersCount = static_cast<int>(inRangeList.size());
    rb->colliders = new RigidBody[rb->collidersCount];
    for (int i = 0; i < rb->collidersCount; i++) {
        rb->colliders[i] = inRangeList[i].rb;
    }
}

int main(int argc, char* args[]) {
    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, "Bieszczady");

    SDL_Surface* screen =
        SDL_CreateRGBSurface(0, display.getWidth(), display.getHeight(), 32,
                             0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_Surface *charset = nullptr, *youdied = nullptr;

    youdied = loadBMP("../bmp/youdied.bmp");

    charset = loadBMP("../bmp/cs8x8.bmp");

    Vector screenMiddle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Vector center(CENTER_X, CENTER_Y);

    printf("Log:\n");

    std::vector<std::vector<SDL_Surface*>> playerAnimations =
        loadSurfaces("../bmp/player", 4, 6, 4, 2, 5, 3, 7, false);

    std::vector<std::vector<SDL_Surface*>> mobAnimations =
        loadSurfaces("../bmp/flyingEye", 8, 0, 0, 0, 7, 4, 2, true);

    SDL_Surface* healthPointTemp = loadBMP("../bmp/red.bmp");
    const int healthPointW = 2;
    const int healthPointH = 20;
    SDL_Surface* healthPoint =
        SDL_CreateRGBSurface(0, healthPointW, healthPointH, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitScaled(healthPointTemp, NULL, healthPoint, NULL);
    SDL_FreeSurface(healthPointTemp);

    std::vector<SDL_Surface*> redSurfaceList;
    redSurfaceList.push_back(loadBMP("../bmp/red.bmp"));
    redSurfaceList.push_back(loadBMP("../bmp/red.bmp"));
    redSurfaceList.push_back(loadBMP("../bmp/red.bmp"));

    std::vector<std::vector<SDL_Surface*>> redSurfaceListList;
    for (int i = 0; i < 8; i++) {
        redSurfaceListList.push_back(redSurfaceList);
    }

    std::vector<SDL_Surface*> boxSurfaceList;
    boxSurfaceList.push_back(loadBMP("../bmp/box.bmp"));

    // int worldWidth, int worldHeight, double noiseValue, double
    // terrainFreq, double caveFreq, float heightMultiplier, float
    // heightAddition, int dirtLayerHeight, unsigned int seed
    const int worldWidth = 600, worldHeight = 100, blockSize = 64;
    int worldSeed = 2137;
    Terrain world(worldWidth, worldHeight, 0.4, 0.05, 0.08, 25, 25, 5,
                  worldSeed);
    world.generate(screen);
    const int worldSize = worldWidth * worldHeight;

    int playerHitboxWidth = 42;
    int playerHitboxHeigth = 60;
    double playerWalkAcceleration = 20;
    double playerMaxSpeed = 30;
    double playerJumpHeight = 45;
    double playerJumpCooldown = 0.5;
    double playerAttackFreq = 0.7;
    int playerMaxHealth = 100;
    bool playerDrawScaledToHitbox = false;
    Vector playerSpawnPoint = world.spawnPoint;

    Alive player(RigidBody(playerSpawnPoint, playerHitboxWidth,
                           playerHitboxHeigth, true, playerMaxSpeed),
                 Weapon(10, Rectangle(54, 64), 500), playerAnimations,
                 playerWalkAcceleration, playerJumpHeight, playerJumpCooldown,
                 playerAttackFreq, playerMaxHealth, 0.2, 0.08, 0.2, 0.3, 0.15,
                 0.15, 0.15);
    player.rndr.setDrawScaledToHitbox(playerDrawScaledToHitbox);

    int wraithHitboxWidth = 64;
    int wraithHitboxHeigth = 64;
    double wraithWalkAcceleration = 10;
    double wraithMaxSpeed = 25;
    double wraithJumpHeight = 35;
    double wraithJumpCooldown = 1;
    double wraithAttackFreq = 1;
    int wraithMaxHealth = 50;

    Alive wraith(
        RigidBody(playerSpawnPoint.add(Vector(0, -300)), wraithHitboxWidth,
                  wraithHitboxHeigth, false, wraithMaxSpeed),
        Weapon(9, Rectangle(60, 60), 10), mobAnimations, wraithWalkAcceleration,
        wraithJumpHeight, wraithJumpCooldown, wraithAttackFreq, wraithMaxHealth,
        0.02, 0.05, 0.1, 0.4);

    GameObject box(Renderer(boxSurfaceList),
                   RigidBody(Vector::ZERO, BLOCK_WIDTH, BLOCK_HEIGHT));

    char text[128];
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
    Vector gravity(0, 10);
    Timer spawnMob;
    spawnMob.setCooldown(5);

    // GAMELOOP
    while (!quit) {
        currentTime = SDL_GetTicks();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        gameDelta = delta * timeFactor;
        if (delta < (1000 / MAX_FPS)) SDL_Delay(1000 / MAX_FPS - delta);
        realTime += delta;
        gameTime += gameDelta;

        player.rb.acceleration = gravity;
        wraith.rb.acceleration = gravity;

        // handle input

        quit = control(&player, realTime / 1000, &wraith, box, world.terrain);
        // change gamestate

        if (wraith.isAlive()) {
            wraith.flyTo(player.getPosition(), realTime / 1000);
            if (Rectangle(100, 100, wraith.getPosition())
                    .contains(player.getPosition())) {
                // ATTACK
                if (wraith.rb.velocity.x >= 0) {
                    wraith.attack(&player, 'R', realTime / 1000);
                } else {
                    wraith.attack(&player, 'L', realTime / 1000);
                }
            }
        } else {
            spawnMob.start(realTime / 1000);
            if (spawnMob.isUp(realTime / 1000)) {
                wraith.alive = true;
                wraith.rb.hitbox.position =
                    player.getPosition().addY(-SCREEN_HEIGHT / 2);
                wraith.health = wraith.maxHealth;
            }
        }

        animationControl(&player, realTime / 1000);
        animationControl(&wraith, realTime / 1000);

        calculateNearbyColliders(&wraith.rb, world.terrain);
        calculateNearbyColliders(&player.rb, world.terrain);

        wraith.rb.collide(gameDelta);
        player.rb.collide(gameDelta);

        wraith.rb.move(gameDelta);
        player.rb.move(gameDelta);

        if (player.getPosition().y > 0 || player.getPosition().x < 0 ||
            player.getPosition().x > worldWidth * BLOCK_WIDTH) {
            // std::vector<GameObject> blockStrip =
            //     world.terrain->queryRange(Rectangle(
            //         100, worldHeight * BLOCK_HEIGHT,
            //         player.getPosition().addY(worldHeight * BLOCK_HEIGHT /
            //         2)));
            // double highestY = 0;
            // for (int i = 0; i < blockStrip.size(); i++) {
            //     if (blockStrip[i].getPosition().y < highestY) {
            //         highestY = blockStrip[i].getPosition().y;
            //     }
            // }
            // player.rb.hitbox.position.y = highestY - 3 * BLOCK_HEIGHT;
            player.rb.hitbox.position = playerSpawnPoint;
        }

        if (!Rectangle(2 * SCREEN_WIDTH, 2 * SCREEN_HEIGHT,
                       player.getPosition())
                 .contains(wraith.getPosition())) {
            wraith.rb.hitbox.position =
                player.getPosition().addY(-SCREEN_HEIGHT / 2);
        }

        // output
        SDL_FillRect(screen, NULL, skyblue);

        camera = player.rb.hitbox.position.difference(screenMiddle);

        std::vector<GameObject> visibleBlocks = world.terrain->queryRange(
            Rectangle(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300,
                      player.rb.hitbox.position));
        for (int i = 0; i < visibleBlocks.size(); i++) {
            visibleBlocks[i].rndr.draw(screen, camera,
                                       visibleBlocks[i].rb.hitbox);
        }
        wraith.rndr.draw(screen, camera, wraith.rb.hitbox);
        player.rndr.draw(screen, camera, player.rb.hitbox);

        DrawRectangle(screen, 40, 4, SCREEN_WIDTH - 80, 20, silver, brown);

        sprintf_s(text,
                  "Czas trwania: %.1lf s    %.0lf klatek / s    Killcount: %d",
                  realTime / 1000, fps, player.killCount);
        DrawString(screen,
                   static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2), 10,
                   text, charset);

        sprintf_s(text, "%d / %d", player.health, player.maxHealth);
        DrawString(screen, 20, SCREEN_HEIGHT - 45, text, charset);
        for (int i = 0; i < player.health; i++) {
            DrawSurface(screen, healthPoint, 20 + (healthPoint->w) * i,
                        SCREEN_HEIGHT - 20);
        }

        if (!player.isAlive()) {
            DrawSurface(screen, youdied, screenMiddle.x, screenMiddle.y / 4);
        }

        display.update(screen);

        // fps counter
        fpsTimer += delta;
        if (fpsTimer > 500) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 500;
        };

        frames++;
    }

    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}