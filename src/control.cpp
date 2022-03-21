#include <SDL.h>

#include "Alive.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "settings.h"

bool control(Alive* entity, double realTime, GameObject block, QuadTree* terrain) {
    if (entity->rb.velocity.y > 10) {
        entity->startAnimation(&entity->falling);
    } else if (entity->rb.velocity.y < 0) {
        entity->startAnimation(&entity->jumping);
    } else if (entity->rb.velocity.x > 10 || entity->rb.velocity.x < -10) {
        entity->startAnimation(&entity->walking);
    } else {
        entity->startAnimation(&entity->idle);
    }

    if (entity->rb.velocity.x > 0) {
        entity->rndr.active->changeSide('R');
    }
    if (entity->rb.velocity.x < 0) {
        entity->rndr.active->changeSide('L');
    }

    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (KeyState[SDL_SCANCODE_ESCAPE]) return true;

    if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_UP] ||
        KeyState[SDL_SCANCODE_SPACE]) {
        entity->jump(realTime);
    }
    if (KeyState[SDL_SCANCODE_D] || KeyState[SDL_SCANCODE_RIGHT]) {
        entity->walk('R');
    }
    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_LEFT]) {
        entity->walk('L');
    }

    entity->rndr.active->changeSurface(realTime);

    int mouseXRelToScreen, mouseYRelToScreen;
    Uint32 buttons;
    buttons = SDL_GetMouseState(&mouseXRelToScreen, &mouseYRelToScreen);
    Vector mousePos = entity->rb.hitbox.position.add(
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
        entity->rb.acceleration =
            entity->rb.acceleration.add(Vector(0, -accel));
    }
    if (KeyState[SDL_SCANCODE_S] || KeyState[SDL_SCANCODE_DOWN]) {
        entity->rb.acceleration = entity->rb.acceleration.add(Vector(0, accel));
    }
    if (KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_LEFT]) {
        entity->rb.acceleration =
            entity->rb.acceleration.add(Vector(-accel, 0));
    }
    if (KeyState[SDL_SCANCODE_D] || KeyState[SDL_SCANCODE_RIGHT]) {
        entity->rb.acceleration = entity->rb.acceleration.add(Vector(accel, 0));
    }
    return false;
}
