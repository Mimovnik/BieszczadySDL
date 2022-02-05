#pragma once
#include <SDL.h>

#include "Rectangle.h"
#include "Vector.h"
class RigidBody {
   public:
    SDL_Surface* surface = nullptr;
    Vector velocity;
    Vector acceleration;
    double accelerationRate;
    double mass;
    Rectangle hitbox;

    RigidBody(){};

    RigidBody(Vector startingPosition, SDL_Surface* surface, int width,
              int height, double accelerationRate);

    void calculatePosition(double gameDelta);

    void draw(SDL_Surface* screen, Vector offset);

    void collide(RigidBody another, double gameDelta);

   protected:
    Rectangle leftHitbox();
    Rectangle rightHitbox();
    Rectangle topHitbox();
    Rectangle bottomHitbox();
};