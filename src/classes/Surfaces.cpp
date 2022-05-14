#include "Surfaces.h"

#include "../functions/loadBMP.h"
#include "../functions/settings.h"

Surfaces::~Surfaces() {
    SDL_FreeSurface(youdied);
    SDL_FreeSurface(charset);
    SDL_FreeSurface(healthPoint);
    SDL_FreeSurface(cursor);
    for(auto& box: boxSurfaces){
        SDL_FreeSurface(box);
    }
    for(auto& playerAnimation: playerAnimations){
        for(auto& surface: playerAnimation){
            SDL_FreeSurface(surface);
        }
    }
    for(auto& mobAnimation: mobAnimations){
        for(auto& surface: mobAnimation){
            SDL_FreeSurface(surface);
        }
    }
}

void Surfaces::load() {
    youdied = loadBMP("bmp/youdied.bmp");

    charset = loadBMP("bmp/cs8x8.bmp");

    playerAnimations =
        loadSurfaces("bmp/player", PLAYER_IDLE_ANIM_FRAMES,
                     PLAYER_WALK_ANIM_FRAMES, PLAYER_JUMP_ANIM_FRAMES,
                     PLAYER_FALL_ANIM_FRAMES, PLAYER_ATTACK1_ANIM_FRAMES,
                     PLAYER_HURT_ANIM_FRAMES, PLAYER_DIE_ANIM_FRAMES, false);

    mobAnimations =
        loadSurfaces("bmp/flyingEye", WRAITH_IDLE_ANIM_FRAMES,
                     WRAITH_WALK_ANIM_FRAMES, WRAITH_JUMP_ANIM_FRAMES,
                     WRAITH_FALL_ANIM_FRAMES, WRAITH_ATTACK1_ANIM_FRAMES,
                     WRAITH_HURT_ANIM_FRAMES, WRAITH_DIE_ANIM_FRAMES, true);

    SDL_Surface* tempRedSquare = loadBMP("bmp/red.bmp");
    healthPoint = SDL_CreateRGBSurface(0, HP_WIDTH, HP_HEIGHT, 32, 0x00FF0000,
                                       0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitScaled(tempRedSquare, NULL, healthPoint, NULL);
    SDL_FreeSurface(tempRedSquare);

    cursor = loadBMP("bmp/cursor.bmp");

    boxSurfaces.push_back(loadBMP("bmp/box.bmp"));
}

SDL_Surface* Surfaces::getYoudied() const { return youdied; }

SDL_Surface* Surfaces::getCharset() const { return charset; }

SDL_Surface* Surfaces::getHealthPoint() const { return healthPoint; }

SDL_Surface* Surfaces::getCursor() const { return cursor; }

std::vector<SDL_Surface*> Surfaces::getBoxSurfaces() const {
    return boxSurfaces;
}

std::vector<std::vector<SDL_Surface*>> Surfaces::getPlayerAnimations() const {
    return playerAnimations;
}

std::vector<std::vector<SDL_Surface*>> Surfaces::getMobAnimations() const {
    return mobAnimations;
}