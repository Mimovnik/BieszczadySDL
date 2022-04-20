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

bool playerControl(Alive* player, double realTime, Alive* creature,
                   GameObject block, QuadTree* terrain) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode Keysym = event.key.keysym.sym;
            if (Keysym == SDLK_ESCAPE) {
                return false;
            }
            if (Keysym == SDLK_1) {
                player->mode = player->fightMode;
            }
            if (Keysym == SDLK_2) {
                player->mode = player->digMode;
            }
            if (Keysym == SDLK_3) {
                player->mode = player->buildMode;
            }
        }
    }
    SDL_PumpEvents();
    const Uint8* KeyState = SDL_GetKeyboardState(NULL);
    if (!player->isAlive()) {
        return true;
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

    char direction;
    Vector blockPos(player->getPosition());
    if (KeyState[SDL_SCANCODE_LEFT] || KeyState[SDL_SCANCODE_J]) {
        direction = 'L';
        blockPos = blockPos.addX(
            -static_cast<int>(BLOCK_WIDTH + player->rb.hitbox.width / 2) - 1);
    }
    if (KeyState[SDL_SCANCODE_RIGHT] || KeyState[SDL_SCANCODE_L]) {
        direction = 'R';
        blockPos = blockPos.addX(
            static_cast<int>(BLOCK_WIDTH + player->rb.hitbox.width / 2) - 1);
    }
    if (KeyState[SDL_SCANCODE_DOWN] || KeyState[SDL_SCANCODE_K]) {
        direction = 'D';
        blockPos = blockPos.addY(
            static_cast<int>(BLOCK_HEIGHT + player->rb.hitbox.height / 2) - 1);
    }
    if (KeyState[SDL_SCANCODE_UP] || KeyState[SDL_SCANCODE_I]) {
        direction = 'U';
        blockPos = blockPos.addY(
            -static_cast<int>(BLOCK_HEIGHT + player->rb.hitbox.height / 2) - 1);
    }

    bool directionKeysPressed =
        KeyState[SDL_SCANCODE_LEFT] || KeyState[SDL_SCANCODE_J] ||
        KeyState[SDL_SCANCODE_RIGHT] || KeyState[SDL_SCANCODE_L] ||
        KeyState[SDL_SCANCODE_DOWN] || KeyState[SDL_SCANCODE_K] ||
        KeyState[SDL_SCANCODE_UP] || KeyState[SDL_SCANCODE_I];
    if (directionKeysPressed) {
        switch (player->mode) {
            case player->fightMode:
                player->attack(creature, direction, realTime);
                break;
            case player->digMode:
                player->dig(blockPos, terrain, realTime);
                break;
            case player->buildMode:
                player->place(block, blockPos, terrain, realTime);
                break;
        }
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

    return true;
}