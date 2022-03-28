#include "controller.h"

#include "../classes/RigidBody.h"
#include "settings.h"

Animation* whichAnimation(Alive* entity) {
    if ((entity->rb.velocity.y > 10)) {
        if (!entity->falling.rightSurfaceList.empty()) {
            return &entity->falling;
        }
    }
    if ((entity->rb.velocity.y < 0)) {
        if (!entity->jumping.rightSurfaceList.empty()) {
            return &entity->jumping;
        }
    }
    if ((entity->rb.velocity.x > 10 || entity->rb.velocity.x < -10)) {
        if (!entity->walking.rightSurfaceList.empty()) {
            return &entity->walking;
        }
    }
    return &entity->idle;
}

void animationControl(Alive* entity, double realTime) {
    if (entity->isAlive()) {
        if (!entity->attacking1.isRunning() && !entity->hurting.isRunning()) {
            entity->startAnimation(whichAnimation(entity));

            if (entity->rb.velocity.x > 0) {
                entity->rndr.active->changeSide('R');
            }
            if (entity->rb.velocity.x < 0) {
                entity->rndr.active->changeSide('L');
            }
        }
    } else if (!entity->dying.isRunning()) {
        entity->startAnimation(&entity->died);
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
    if (KeyState[SDL_SCANCODE_LEFT] || KeyState[SDL_SCANCODE_J]) {
        player->attack(creature, 'L', realTime);
    } else if (KeyState[SDL_SCANCODE_RIGHT] || KeyState[SDL_SCANCODE_L]) {
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