#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>

#include <cstring>

#include "../classes/Vector.h"

// narysowanie napisu txt na powierzchni screen, zaczynając od punktu (x, y)
// charset to bitmapa 128x128 zawierająca znaki
void DrawString(SDL_Surface* screen, double x, double y, const char* text,
                SDL_Surface* charset);

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, double x, double y);

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie w odniesieniu do kamery
// offset
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, double x, double y,
                 Vector offset);

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

Uint32 getPixel(SDL_Surface* surface, int x, int y);

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

// rysowanie linii o długości l w pionie (gdy dx = 0, dy = 1)
// bądź poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy,
              Uint32 color);

// rysowanie prostokąta o długości boków l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor);

#endif