#include "loadBMP.h"

#include "../classes/Framework.h"
#include "draw.h"

SDL_Surface* loadBMP(std::string fileName) {
    SDL_Surface* surface = SDL_LoadBMP(fileName.c_str());
    if (surface == NULL) {
        printf("SDL_LoadBMP(%s) error: %s\n", fileName.c_str(), SDL_GetError());
        throw 1;
    };
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
    return surface;
}

SDL_Surface* createFlipV(SDL_Surface* surface) {
    SDL_Surface* flipped =
        SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);
    SDL_LockSurface(flipped);

    int pitch = flipped->pitch;    // row size
    char* temp = new char[pitch];  // intermediate buffer
    char* pixels = (char*)flipped->pixels;

    for (int i = 0; i < flipped->h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (flipped->h - i - 1) * pitch;

        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }

    delete[] temp;

    SDL_UnlockSurface(flipped);
    return flipped;
}

SDL_Surface* createFlipH(SDL_Surface* surface) {
    SDL_Surface* flipped =
        SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);
    SDL_LockSurface(flipped);
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            Uint32 pixel = getPixel(surface, surface->w - x - 1, y);
            setPixel(flipped, x, y, pixel);
        }
    }
    SDL_UnlockSurface(flipped);
    return flipped;
}

std::vector<std::vector<SDL_Surface*>> loadSurfaces(
    std::string folder, const int idleCount, const int walkCount,
    const int jumpCount, const int fallCount, const int attack1Count, const int attackUpCount,
    const int hurtCount, const int dieCount, bool idleIsFly) {
    std::vector<std::vector<SDL_Surface*>> surfaces;

    // IDLE OR FLY
    std::vector<SDL_Surface*> idleRight;
    if (idleIsFly) {
        // FLY

        for (int i = 0; i < idleCount; i++) {
            idleRight.push_back(
                loadBMP(folder + "/fly/fly-" + std::to_string(i) + ".bmp"));
        }

    } else {
        // IDLE
        for (int i = 0; i < idleCount; i++) {
            idleRight.push_back(
                loadBMP(folder + "/idle/idle-" + std::to_string(i) + ".bmp"));
        }
    }
    std::vector<SDL_Surface*> idleLeft;
    for (int i = 0; i < idleRight.size(); i++) {
        idleLeft.push_back(createFlipH(idleRight[i]));
    }

    // WALK
    std::vector<SDL_Surface*> walkRight;
    for (int i = 0; i < walkCount; i++) {
        walkRight.push_back(
            loadBMP(folder + "/run/run-" + std::to_string(i) + ".bmp"));
    }
    std::vector<SDL_Surface*> walkLeft;
    for (int i = 0; i < walkRight.size(); i++) {
        walkLeft.push_back(createFlipH(walkRight[i]));
    }

    // JUMP
    std::vector<SDL_Surface*> jumpRight;
    for (int i = 0; i < jumpCount; i++) {
        jumpRight.push_back(
            loadBMP(folder + "/jump/jump-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> jumpLeft;
    for (int i = 0; i < jumpRight.size(); i++) {
        jumpLeft.push_back(createFlipH(jumpRight[i]));
    }

    // FALL

    std::vector<SDL_Surface*> fallRight;
    for (int i = 0; i < fallCount; i++) {
        fallRight.push_back(
            loadBMP(folder + "/fall/fall-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> fallLeft;
    for (int i = 0; i < fallRight.size(); i++) {
        fallLeft.push_back(createFlipH(fallRight[i]));
    }

    // ATTACK

    std::vector<SDL_Surface*> attack1Right;
    for (int i = 0; i < attack1Count; i++) {
        attack1Right.push_back(
            loadBMP(folder + "/attack1/attack1-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> attack1Left;
    for (int i = 0; i < attack1Right.size(); i++) {
        attack1Left.push_back(createFlipH(attack1Right[i]));
    }

    std::vector<SDL_Surface*> attackUpRight;
    for (int i = 0; i < attackUpCount; i++) {
        attackUpRight.push_back(
            loadBMP(folder + "/attackUp/attackUpp-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> attackUpLeft;
    for (int i = 0; i < attackUpRight.size(); i++) {
        attackUpLeft.push_back(createFlipH(attackUpRight[i]));
    }

    // HURT

    std::vector<SDL_Surface*> hurtRight;
    for (int i = 0; i < hurtCount; i++) {
        hurtRight.push_back(
            loadBMP(folder + "/hurt/hurt-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> hurtLeft;
    for (int i = 0; i < hurtRight.size(); i++) {
        hurtLeft.push_back(createFlipH(hurtRight[i]));
    }

    // DIE

    std::vector<SDL_Surface*> dieRight;
    for (int i = 0; i < dieCount; i++) {
        dieRight.push_back(
            loadBMP(folder + "/die/die-" + std::to_string(i) + ".bmp"));
    }

    std::vector<SDL_Surface*> dieLeft;
    for (int i = 0; i < dieRight.size(); i++) {
        dieLeft.push_back(createFlipH(dieRight[i]));
    }

    // DIED

    std::vector<SDL_Surface*> diedRight;
    diedRight.push_back(loadBMP(folder + "/died/died.bmp"));

    std::vector<SDL_Surface*> diedLeft;
    for (int i = 0; i < diedRight.size(); i++) {
        diedLeft.push_back(createFlipH(diedRight[i]));
    }
        
    // ADD TO GENERAL LIST
    surfaces.push_back(idleLeft);      // 0
    surfaces.push_back(idleRight);     // 1
    surfaces.push_back(walkLeft);      // 2
    surfaces.push_back(walkRight);     // 3
    surfaces.push_back(jumpLeft);      // 4
    surfaces.push_back(jumpRight);     // 5
    surfaces.push_back(fallLeft);      // 6
    surfaces.push_back(fallRight);     // 7
    surfaces.push_back(attack1Left);   // 8
    surfaces.push_back(attack1Right);  // 9
    surfaces.push_back(attackUpLeft);  // 10
    surfaces.push_back(attackUpRight); // 11
    surfaces.push_back(hurtLeft);      // 12
    surfaces.push_back(hurtRight);     // 13
    surfaces.push_back(dieLeft);       // 14
    surfaces.push_back(dieRight);      // 15
    surfaces.push_back(diedLeft);      // 16
    surfaces.push_back(diedRight);     // 17

    return surfaces;
}