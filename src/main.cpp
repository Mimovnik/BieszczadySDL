﻿
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <time.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "classes/Alive.h"
#include "classes/Animation.h"
#include "classes/Framework.h"
#include "classes/GameObject.h"
#include "classes/Rectangle.h"
#include "classes/RigidBody.h"
#include "classes/Surfaces.h"
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
    if (terrain == nullptr) {
        return;
    }

    std::vector<GameObject> inRangeList = terrain->queryRange(Rectangle(
        rb->hitbox.width + 200, rb->hitbox.height + 200, rb->hitbox.position));
    rb->collidersCount = static_cast<int>(inRangeList.size());
    rb->colliders = new RigidBody[rb->collidersCount];
    for (int i = 0; i < rb->collidersCount; i++) {
        rb->colliders[i] = inRangeList[i].rb;
    }
    inRangeList.clear();
    inRangeList.shrink_to_fit();
}

void restartRun(double* realTime, double* gameTime, Timer* spawnMob,
                Timer* onDeath, bool* inRun, Alive* player, Alive* mob) {
    *realTime = 0;
    *gameTime = 0;
    *inRun = true;

    player->alive = true;
    player->health = player->maxHealth;
    player->rb.acceleration = Vector::ZERO;
    player->rb.velocity = Vector::ZERO;
    // RESET TIMERS
    spawnMob->reset();
    onDeath->reset();

    player->jumpTimer.reset();
    player->digDelay.reset();
    player->placeDelay.reset();
    player->attackFreq.reset();
    player->idle.nextSprite.reset();
    player->walking.nextSprite.reset();
    player->jumping.nextSprite.reset();
    player->falling.nextSprite.reset();
    player->attacking1.nextSprite.reset();
    player->hurting.nextSprite.reset();
    player->dying.nextSprite.reset();
    player->died.nextSprite.reset();

    player->killCount = 0;
    mob->health = mob->maxHealth;

    mob->attackFreq.reset();
    mob->idle.nextSprite.reset();
    mob->walking.nextSprite.reset();
    mob->jumping.nextSprite.reset();
    mob->falling.nextSprite.reset();
    mob->attacking1.nextSprite.reset();
    mob->hurting.nextSprite.reset();
    mob->dying.nextSprite.reset();
    mob->died.nextSprite.reset();
}

int main(int argc, char* args[]) {
    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, "Bieszczady");

    SDL_Surface* screen =
        SDL_CreateRGBSurface(0, display.getWidth(), display.getHeight(), 32,
                             0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    Surfaces surfaces;
    surfaces.load();

    Vector screenMiddle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // World initialization
    srand(time(NULL));
    int worldSeed = rand() % 10000;

    Terrain* world =
        new Terrain(WORLD_WIDTH, WORLD_HEIGHT, CAVE_HOLLOWNESS,
                    TERRAIN_RUGGEDNESS, CAVE_DENSITY, TERRAIN_STEEPNESS,
                    TERRAIN_HEIGHT, TERRAIN_DIRTLAYER, worldSeed);
    world->generate(screen);
    const int worldSize = WORLD_WIDTH * WORLD_HEIGHT;

    Vector playerSpawnPoint = world->spawnPoint;

    Alive player(
        RigidBody(playerSpawnPoint, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT,
                  true, PLAYER_MAX_SPEED),
        Weapon(10, Rectangle(54, 64), 500), Tool(5, 1, Rectangle(200, 200), 5),
        surfaces.getPlayerAnimations(), PLAYER_WALK_ACCEL_RATE,
        PLAYER_JUMP_HEIGHT, PLAYER_JUMP_COOLDOWN, PLAYER_ATTACK_COOLDOWN,
        PLAYER_MAX_HEALTH, PLAYER_IDLE_ANIM_FREQ, PLAYER_ATTACK1_ANIM_FREQ,
        PLAYER_HURT_ANIM_FREQ, PLAYER_DIE_ANIM_FREQ, PLAYER_WALK_ANIM_FREQ,
        PLAYER_JUMP_ANIM_FREQ, PLAYER_FALL_ANIM_FREQ);
    player.rndr.setDrawScaledToHitbox(false);

    Alive wraith(
        RigidBody(playerSpawnPoint.add(Vector(0, -300)), WRAITH_HITBOX_WIDTH,
                  WRAITH_HITBOX_HEIGHT, false, WRAITH_MAX_SPEED),
        Weapon(9, Rectangle(60, 60), 20), Tool(), surfaces.getMobAnimations(),
        WRAITH_WALK_ACCEL_RATE, WRAITH_JUMP_HEIGHT, WRAITH_JUMP_COOLDOWN,
        WRAITH_ATTACK_COOLDOWN, WRAITH_MAX_HEALTH, WRAITH_IDLE_ANIM_FREQ,
        WRAITH_ATTACK1_ANIM_FREQ, WRAITH_ATTACKUP_ANIM_FREQ,
        WRAITH_HURT_ANIM_FREQ, WRAITH_DIE_ANIM_FREQ);

    GameObject box(Renderer(surfaces.getBoxSurfaces()),
                   RigidBody(Vector::ZERO, BLOCK_WIDTH, BLOCK_HEIGHT),
                   BOX_HEALTH);

    char text[128];
    int black = SDL_MapRGB(screen->format, 0, 0, 0);
    int silver = SDL_MapRGB(screen->format, 192, 192, 192);
    int green = SDL_MapRGB(screen->format, 0, 153, 51);
    int red = SDL_MapRGB(screen->format, 204, 0, 0);
    int skyblue = SDL_MapRGB(screen->format, 153, 240, 255);
    int brown = SDL_MapRGB(screen->format, 102, 51, 0);

    int frames = 0;
    double fpsTimer = 0, fps = 0, realTime = 0, gameTime = 0, timeFactor = 0.01;
    int currentTime, delta;
    int lastTime = SDL_GetTicks();
    double runTime = 0;
    double gameDelta;

    bool inRun = true;
    bool quit = false;
    bool wantQuit = false;

    Vector camera;
    Vector gravity(0, 10);
    Timer spawnMob;
    spawnMob.setCooldown(5);
    Timer postDeath;
    postDeath.setCooldown(5);

    // GAMELOOP
    while (!quit) {
        currentTime = SDL_GetTicks();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        gameDelta = delta * timeFactor;
        if (delta < (1000 / MAX_FPS)) SDL_Delay(1000 / MAX_FPS - delta);
        SDL_FillRect(screen, NULL, skyblue);

        if (inRun) {
            realTime += delta;
            gameTime += gameDelta;
            runTime = realTime / 1000;

            player.rb.acceleration = gravity;
            wraith.rb.acceleration = gravity;

            // handle input

            inRun = playerControl(&player, realTime / 1000, &wraith, box,
                                  world->terrain);
            // change gamestate

            if (wraith.isAlive()) {
                wraith.flyTo(player.getPosition(), realTime / 1000);
                if (Rectangle(100, 100, wraith.getPosition())
                        .contains(player.getPosition())) {
                    // ATTACK
                    if (wraith.rb.velocity.x >= 0) {
                        wraith.faceDirection = 'R';
                    } else {
                        wraith.faceDirection = 'L';
                    }
                    wraith.attack1(&player, realTime / 1000);
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

            calculateNearbyColliders(&wraith.rb, world->terrain);
            calculateNearbyColliders(&player.rb, world->terrain);

            wraith.rb.collide(gameDelta);
            player.rb.collide(gameDelta);

            wraith.rb.move(gameDelta);
            player.rb.move(gameDelta);

            if (player.getPosition().y > 0 || player.getPosition().x < 0 ||
                player.getPosition().x > WORLD_WIDTH * BLOCK_WIDTH) {
                player.rb.hitbox.position = playerSpawnPoint;
            }

            if (!Rectangle(2 * SCREEN_WIDTH, 2 * SCREEN_HEIGHT,
                           player.getPosition())
                     .contains(wraith.getPosition())) {
                wraith.rb.hitbox.position =
                    player.getPosition().addY(-SCREEN_HEIGHT / 2);
            }
        }

        // output

        camera = player.rb.hitbox.position.difference(screenMiddle);

        std::vector<GameObject> visibleBlocks = world->terrain->queryRange(
            Rectangle(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300,
                      player.rb.hitbox.position));
        for (int i = 0; i < visibleBlocks.size(); i++) {
            visibleBlocks[i].rndr.draw(screen, camera,
                                       visibleBlocks[i].rb.hitbox);
        }
        visibleBlocks.clear();
        visibleBlocks.shrink_to_fit();
        wraith.rndr.draw(screen, camera, wraith.rb.hitbox);
        player.rndr.draw(screen, camera, player.rb.hitbox);

        DrawRectangle(screen, 40, 4, SCREEN_WIDTH - 80, 20, silver, brown);
        std::string modeName;
        switch (player.mode) {
            case player.fightMode:
                modeName = "fight";
                break;
            case player.digMode:
                modeName = "dig";
                break;
            case player.buildMode:
                modeName = "build";
                break;
        }
        sprintf(text,
                "Mode: %s    Run duration: %.1lf s    %.0lf fps    "
                "Killcount: %d",
                modeName.c_str(), realTime / 1000, fps, player.killCount);
        DrawString(screen,
                   static_cast<int>(screen->w / 2 - strlen(text) * 8 / 2), 10,
                   text, surfaces.getCharset());

        sprintf(text, "%d / %d", player.health, player.maxHealth);
        DrawString(screen, 20, SCREEN_HEIGHT - 45, text, surfaces.getCharset());
        for (int i = 0; i < player.health; i++) {
            DrawSurface(screen, surfaces.getHealthPoint(),
                        20 + (surfaces.getHealthPoint()->w) * i,
                        SCREEN_HEIGHT - 20);
        }

        if (!player.isAlive()) {
            DrawSurface(screen, surfaces.getYoudied(), screenMiddle.x,
                        screenMiddle.y / 4);

            postDeath.start(realTime / 1000);
            if (postDeath.isUp(realTime / 1000)) {
                inRun = false;
            }
        }

        // MENU
        if (!inRun) {
            // SCORE
            DrawRectangle(screen, static_cast<int>(screenMiddle.x - 200),
                          static_cast<int>(screenMiddle.y - 105), 400, 20,
                          silver, brown);

            sprintf(text, "You survived: %.1lf s and killed: %d mobs.", runTime,
                    player.killCount);
            DrawString(screen,
                       static_cast<int>(screenMiddle.x - strlen(text) * 8 / 2),
                       screenMiddle.y - 100, text, surfaces.getCharset());

            // info
            DrawRectangle(screen, static_cast<int>(screenMiddle.x - 200),
                          static_cast<int>(screenMiddle.y - 55), 400, 20,
                          silver, brown);

            sprintf(text, "Press escape to exit, press R to start another run");
            DrawString(screen,
                       static_cast<int>(screenMiddle.x - strlen(text) * 8 / 2),
                       screenMiddle.y - 50, text, surfaces.getCharset());

            SDL_FlushEvent(SDL_KEYDOWN);
            SDL_Event event;

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    wantQuit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    SDL_Keycode Keysym = event.key.keysym.sym;
                    if (Keysym == SDLK_ESCAPE) {
                        wantQuit = true;
                    }
                    if (Keysym == SDLK_r) {
                        restartRun(&realTime, &gameTime, &spawnMob, &postDeath,
                                   &inRun, &player, &wraith);
                        delete world;
                        const int worldWidth = 600, worldHeight = 100,
                                  blockSize = 64;
                        srand(time(NULL));
                        worldSeed = rand() % 10000;
                        world = new Terrain(worldWidth, worldHeight, 0.4, 0.05,
                                            0.08, 25, 25, 5, worldSeed);
                        world->generate(screen);
                        playerSpawnPoint = world->spawnPoint;
                        player.rb.hitbox.position = playerSpawnPoint;
                        wraith.rb.hitbox.position =
                            player.getPosition().addY(-500);
                    }
                    if (wantQuit) {
                        if (Keysym == SDLK_y) {
                            quit = true;
                        }
                        if (Keysym == SDLK_n) {
                            wantQuit = false;
                            if (player.isAlive()) {
                                inRun = true;
                            }
                        }
                    }
                }
            }

            if (wantQuit) {
                DrawRectangle(screen, static_cast<int>(screenMiddle.x - 200),
                              static_cast<int>(screenMiddle.y - 5), 400, 20,
                              silver, brown);

                sprintf(text, "Are you sure you want to quit? [y/n]");
                DrawString(
                    screen,
                    static_cast<int>(screenMiddle.x - strlen(text) * 8 / 2),
                    screenMiddle.y, text, surfaces.getCharset());
            }
        }
        if (player.mode != player.Mode::fightMode) {
            DrawSurface(screen, surfaces.getCursor(),
                        player.getPosition().x + player.actionCursor.x,
                        player.getPosition().y + player.actionCursor.y, camera);
        }
        // draw everything
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

    // surfaces.~Surfaces();
    SDL_Quit();
    return 0;
}