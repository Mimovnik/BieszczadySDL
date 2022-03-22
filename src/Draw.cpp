#include <SDL.h>

#include <cstring>

#include "Vector.h"

#ifndef DRAW_CPP
#define DRAW_CPP

// narysowanie napisu txt na powierzchni screen, zaczynając od punktu (x, y)
// charset to bitmapa 128x128 zawierająca znaki
void DrawString(SDL_Surface* screen, double x, double y, const char* text,
                SDL_Surface* charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = static_cast<int>(x);
        d.y = static_cast<int>(y);
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    };
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, double x, double y) {
    SDL_Rect dest;
    dest.x = static_cast<int>(x - sprite->w / 2);
    dest.y = static_cast<int>(y - sprite->h / 2);
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie w odniesieniu do kamery
// offset
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, double x, double y,
                 Vector offset) {
    SDL_Rect dest;
    dest.x = static_cast<int>(x - sprite->w / 2 - offset.x);
    dest.y = static_cast<int>(y - sprite->h / 2 - offset.y);
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    Uint32* const target_pixel =
        (Uint32*)((Uint8*)surface->pixels + y * surface->pitch +
                  x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}

Uint32 getPixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16*)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32*)p;
            break;

        default:
            return 0; /* shouldn't happen, but avoids warnings */
    }
}

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    SDL_LockSurface(surface);
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32*)p = color;
    SDL_UnlockSurface(surface);
};


SDL_Surface* createFlipV(SDL_Surface* surface) {
    SDL_Surface* flipped = SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);
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
    SDL_Surface* flipped = SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);
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

// rysowanie linii o długości l w pionie (gdy dx = 0, dy = 1)
// bądź poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy,
              Uint32 color) {
    for (int i = 0; i < l; i++) {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};

// rysowanie prostokąta o długości boków l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
    int i;
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

#endif