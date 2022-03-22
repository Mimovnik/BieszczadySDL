#include <SDL.h>

#include "Alive.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "settings.h"

void animationControl(Alive* entity, double realTime) {
    if (entity->rb.velocity.y > 10) {
        if (!entity->falling.rightSurfaceList.empty()) {
            entity->startAnimation(&entity->falling);
        }
    } else if (entity->rb.velocity.y < 0) {
        if (!entity->falling.rightSurfaceList.empty()) {
            entity->startAnimation(&entity->jumping);
        }
    } else if (entity->rb.velocity.x > 10 || entity->rb.velocity.x < -10) {
        if (!entity->falling.rightSurfaceList.empty()) {
            entity->startAnimation(&entity->walking);
        }
    } else {
        entity->startAnimation(&entity->idle);
    }

    if (entity->rb.velocity.x > 0) {
        entity->rndr.active->changeSide('R');
    }
    if (entity->rb.velocity.x < 0) {
        entity->rndr.active->changeSide('L');
    }

    entity->rndr.active->changeSurface(realTime);
}

bool control(Alive* player, double realTime, Alive* creature, GameObject block,
             QuadTree* terrain) {
    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (KeyState[SDL_SCANCODE_ESCAPE]) return true;

    if (!player->isAlive()) {
        return false;
    }

    if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_SPACE]) {
        player->jump(realTime);
    }
    if (KeyState[SDL_SCANCODE_D]) {
        player->walk('R');
    }
    if (KeyState[SDL_SCANCODE_A]) {
        player->walk('L');
    }
    if (KeyState[SDL_SCANCODE_LEFT]) {
        player->attack(creature, 'L', realTime);
    } else if (KeyState[SDL_SCANCODE_RIGHT]) {
        player->attack(creature, 'R', realTime);
    }

    int mouseXRelToScreen, mouseYRelToScreen;
    Uint32 buttons;
    buttons = SDL_GetMouseState(&mouseXRelToScreen, &mouseYRelToScreen);
    Vector mousePos = player->rb.hitbox.position.add(
        Vector(mouseXRelToScreen, mouseYRelToScreen)
            .difference(Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)));

    if ((buttons & SDL_BUTTON_RMASK) != 0) {
        player->place(block, mousePos, terrain, realTime);
    }
    if ((buttons & SDL_BUTTON_LMASK) != 0) {
        player->dig(mousePos, terrain, realTime);
    }

    return false;
}
