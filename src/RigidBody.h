#pragma once
#include <SDL.h>

#include "Rectangle.h"
#include "Vector.h"
class RigidBody {
   public:
    SDL_Surface* surface = nullptr;
    bool drawScaledToHitbox;
    Vector velocity;
    Vector acceleration;
    double mass;
    Rectangle hitbox;
    double maxSpeed;

    RigidBody(){};

    RigidBody(Vector startingPosition, SDL_Surface* surface, int width,
              int height, bool drawScaledToHitbox = true, double maxSpeed = 10);

    void calculatePosition(double gameDelta, RigidBody* others, int othersCount);

    void draw(SDL_Surface* screen, Vector offset);

    void collide(RigidBody* others, int othersCount, double gameDelta);

   protected:
    Rectangle leftHitbox();
    Rectangle rightHitbox();
    Rectangle topHitbox();
    Rectangle bottomHitbox();
};