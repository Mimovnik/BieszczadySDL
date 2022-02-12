#include <SDL.h>

#include "Alive.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "settings.h"

bool control(Alive* entity, double realTime, RigidBody* colliders,
             int collidersCount, RigidBody block, QuadTree* terrain) {
    if (entity->velocity.magnitude() < 10 && entity->velocity.magnitude() > -10) {
        entity->startAnimation(&entity->idle);
    }
    if (entity->active.leftSurfaceList != entity->jumping.leftSurfaceList && entity->velocity.y < 10) {
        if (entity->velocity.x > 10 || entity->velocity.x < -10) {
            entity->startAnimation(&entity->walking);
        }
    }

    if (entity->velocity.y > 10) {
        entity->startAnimation(&entity->falling);
    }

    if (entity->velocity.x > 0) {
        entity->active.changeSide('R');
    }
    if (entity->velocity.x < 0) {
        entity->active.changeSide('L');
    }

    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (KeyState[SDL_SCANCODE_ESCAPE]) return true;

    if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_UP] ||
        KeyState[SDL_SCANCODE_SPACE]) {
        entity->jump(colliders, collidersCount, realTime);
        entity->startAnimation(&entity->jumping);
    }
    if (KeyState[SDL_SCANCODE_D] || KeyState[SDL_SCANCODE_RIGHT]) {
        entity->walk('R', colliders, collidersCount);
        entity->active.changeSide('R');
    }
    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_LEFT]) {
        entity->walk('L', colliders, collidersCount);
        entity->active.changeSide('L');
    }

    entity->active.changeSurface(realTime);

    int mouseXRelToScreen, mouseYRelToScreen;
    Uint32 buttons;
    buttons = SDL_GetMouseState(&mouseXRelToScreen, &mouseYRelToScreen);
    Vector mousePos = entity->hitbox.position.add(
        Vector(mouseXRelToScreen, mouseYRelToScreen)
            .difference(Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)));

    if ((buttons & SDL_BUTTON_RMASK) != 0) {
        entity->place(block, mousePos, terrain, realTime);
    }
    if ((buttons & SDL_BUTTON_LMASK) != 0) {
        entity->dig(mousePos, terrain, realTime);
    }

    return false;
}

bool noclip(Alive* entity) {
    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (KeyState[SDL_SCANCODE_ESCAPE]) return true;

    double accel = 10;
    if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_UP]) {
        entity->acceleration = entity->acceleration.add(Vector(0, -accel));
    }
    if (KeyState[SDL_SCANCODE_S] || KeyState[SDL_SCANCODE_DOWN]) {
        entity->acceleration = entity->acceleration.add(Vector(0, accel));
    }
    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_LEFT]) {
        entity->acceleration = entity->acceleration.add(Vector(-accel, 0));
    }
    if (KeyState[SDL_SCANCODE_D] || KeyState[SDL_SCANCODE_RIGHT]) {
        entity->acceleration = entity->acceleration.add(Vector(accel, 0));
    }
    return false;
}
