#define _USE_MATH_DEFINES
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>

#include <iostream>

#include "src/Alive.h"
#include "src/Draw.cpp"
#include "src/Rectangle.h"
#include "src/RigidBody.h"
#include "src/Vector.h"

#define FULLSCREEN_ON false
#define MAX_FPS 150
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void displaySetUp(SDL_Surface** charset, SDL_Surface** screen,
                  SDL_Texture** screenTexture, SDL_Window** window,
                  SDL_Renderer** renderer) {
    char errorMessage[128];
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        sprintf_s(errorMessage, "SDL_Init error: %s\n", SDL_GetError());
        throw errorMessage;
    }

    // tryb pe³noekranowy / fullscreen mode
    int rendererCreated;
    if (FULLSCREEN_ON) {
        rendererCreated = SDL_CreateWindowAndRenderer(
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, window, renderer);
    } else {
        rendererCreated = SDL_CreateWindowAndRenderer(
            SCREEN_WIDTH, SCREEN_HEIGHT, 0, window, renderer);
    }

    if (rendererCreated != 0) {
        SDL_Quit();
        sprintf_s(errorMessage, "SDL_CreateWindowAndRenderer error: %s\n",
                  SDL_GetError());
        throw errorMessage;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);

    SDL_SetWindowTitle(*window, "Bieszczady");

    *screen =
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);

    *screenTexture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       SCREEN_WIDTH, SCREEN_HEIGHT);

    // wczytanie obrazka cs8x8.bmp
    *charset = SDL_LoadBMP("../bmp/cs8x8.bmp");
    if (*charset == NULL) {
        sprintf_s(errorMessage, "SDL_LoadBMP(cs8x8.bmp) error: %s\n",
                  SDL_GetError());
        SDL_FreeSurface(*screen);
        SDL_DestroyTexture(*screenTexture);
        SDL_DestroyWindow(*window);
        SDL_DestroyRenderer(*renderer);
        SDL_Quit();
        throw errorMessage;
    };
    SDL_SetColorKey(*charset, true, 0x000000);
}

SDL_Surface* loadBMP(const char* fileName, SDL_Surface* charset,
                     SDL_Surface* screen, SDL_Texture* scrtex,
                     SDL_Window* window, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(fileName);
    if (surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        throw 1;
    };
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
    return surface;
}

bool control(Alive* entity, double realTime, RigidBody colObj1,
             RigidBody* boxes, int boxesCount) {
    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (KeyState[SDL_SCANCODE_ESCAPE]) return true;

    if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_UP] ||
        KeyState[SDL_SCANCODE_SPACE]) {
        entity->jump(colObj1, realTime);
        entity->jump(boxes, boxesCount, realTime);
    }
    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_LEFT]) {
        entity->move('R', colObj1);
        entity->move('R', boxes, boxesCount);
    }
    if (KeyState[SDL_SCANCODE_D] || KeyState[SDL_SCANCODE_RIGHT]) {
        entity->move('L', colObj1);
        entity->move('L', boxes, boxesCount);
    }
    return false;
}

int main(int argc, char* args[]) {
    SDL_Surface* screen = nullptr;
    SDL_Texture* screenTexture = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Surface *charset = nullptr, *theme = nullptr;

    Vector screenMiddle = Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    try {
        displaySetUp(&charset, &screen, &screenTexture, &window, &renderer);
    } catch (char* error) {
        std::cerr << error;
        return 1;
    }

    printf("Log:\n");

    SDL_Surface* heroSurface = loadBMP("../bmp/witcher200.bmp", charset, screen,
                                       screenTexture, window, renderer);

    int hitboxWidth = 60;
    int hitboxHeigth = 88;
    double walkAcceleration = 10;
    double maxSpeed = 30;
    double jumpHeight = 40;
    double jumpCooldown = 0.5;
    Alive player = Alive(screenMiddle, heroSurface, hitboxWidth, hitboxHeigth,
                         maxSpeed, walkAcceleration, jumpHeight, jumpCooldown);

    Vector floorPosition = Vector(320, 750);
    SDL_Surface* floorSurface = loadBMP("../bmp/floor.bmp", charset, screen,
                                        screenTexture, window, renderer);
    RigidBody floor = RigidBody(floorPosition, floorSurface, 1920, 63);

    const int boxesCount = 32;
    Vector boxesPositions[boxesCount];
    RigidBody boxes[boxesCount];
    SDL_Surface* boxSurface = loadBMP("../bmp/box.bmp", charset, screen,
                                      screenTexture, window, renderer);
    for (int i = 0; i < boxesCount; i++) {
        //boxes' positions setup
        if (i < boxesCount / 4) {
            boxesPositions[i] = screenMiddle.add(Vector(i * 64, 128));
        } else if (i < 2 * boxesCount / 4) {
            boxesPositions[i] =
                screenMiddle.add(Vector((i - 2) * 64, 192));
        } else if (i < 3 * boxesCount / 4) {
            boxesPositions[i] =
                screenMiddle.add(Vector((i - 13) * 64, 256));
        } else {
            boxesPositions[i] =
                screenMiddle.add(Vector((i - 17) * 64, 320));
        }
        boxes[i] = RigidBody(boxesPositions[i], boxSurface, 64, 64);
    }

    theme = loadBMP("../bmp/theme.bmp", charset, screen, screenTexture, window,
                    renderer);

    char text[128];
    int black = SDL_MapRGB(screen->format, 0, 0, 0);
    int silver = SDL_MapRGB(screen->format, 192, 192, 192);
    int green = SDL_MapRGB(screen->format, 0, 153, 51);
    int red = SDL_MapRGB(screen->format, 204, 0, 0);
    int blue = SDL_MapRGB(screen->format, 51, 204, 255);
    int brown = SDL_MapRGB(screen->format, 102, 51, 0);

    int lastTime = 0, frames = 0;
    int currentTime, delta;
    double fpsTimer = 0, fps = 0, realTime = 0, gameTime = 0, timeFactor = 0.01;
    double gameDelta;
    bool quit = false;
    lastTime = SDL_GetTicks();
    Vector camera;

    double speed = 0.5;
    Vector gravity = Vector(0, 10);

    while (!quit) {
        currentTime = SDL_GetTicks();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        gameDelta = delta * timeFactor;
        if (delta < (1000 / MAX_FPS)) SDL_Delay(1000 / MAX_FPS - delta);

        realTime += delta;
        gameTime += gameDelta;

        SDL_FillRect(screen, NULL, black);

        camera = player.hitbox.position.difference(screenMiddle);

        DrawSurface(screen, theme, screenMiddle.x, screenMiddle.y, camera);

        player.acceleration = gravity;
        for (int i = 0; i < boxesCount; i++) {
            boxes[i].acceleration = gravity;
        }

        quit = control(&player, realTime / 1000, floor, boxes, boxesCount);

        player.collide(floor, gameDelta);
        player.collide(boxes, boxesCount, gameDelta);

        for (int i = 0; i < boxesCount; i++) {
            boxes[i].collide(floor, gameDelta);
            boxes[i].collide(boxes, boxesCount, gameDelta);
        }
        // dont collide with yourself

        floor.draw(screen, camera);
        for (int i = 0; i < boxesCount; i++) {
            boxes[i].draw(screen, camera);
        }
        player.draw(screen, camera);

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
        DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text,
                   charset);
        //	      "Esc - exit, \030 - faster, \031 - slower"
        sprintf_s(text,
                  "Esc - wyjscie, W / \030 - skok, A / \032 oraz D / \033 - "
                  "sterowanie");
        DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text,
                   charset);

        SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch);

        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // obsługa zdarzeń (o ile jakieś zaszły)

        player.calculatePosition(gameDelta, boxes, boxesCount);
        for (int i = 0; i < boxesCount; i++) {
            boxes[i].calculatePosition(gameDelta, floor);
        }

        printf(
            "Player's position: x = %.2f y = %.2f "
            "velocity: x = %.8f y = %.8f "
            "acceleration: x = %.4f y = %.4f\n",
            player.hitbox.position.x, player.hitbox.position.y,
            player.velocity.x, player.velocity.y, player.acceleration.x,
            player.acceleration.y);

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