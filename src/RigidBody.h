#pragma once
#include <SDL.h>

#include "Rectangle.h"
#include "Vector.h"
#include <vector>
#include "Timer.h"
class RigidBody {
   public:
    std::vector<SDL_Surface*> currentSurfaceList;
    int currentSurfaceIndex;
    Timer animation;
    bool drawScaledToHitbox;
    bool collidable;
    Vector velocity;
    Vector acceleration;
    double mass;
    Rectangle hitbox;
    double maxSpeed;

    RigidBody(){};

    RigidBody(Vector startingPosition, std::vector<SDL_Surface*> idleSurfaceList, int width,
              int height, bool collidable = true, bool drawScaledToHitbox = true, double maxSpeed = 10);

    void calculatePosition(double gameDelta, RigidBody* others, int othersCount);

    void draw(SDL_Surface* screen, Vector offset, double realTime);

    void collide(RigidBody* others, int othersCount, double gameDelta);

   protected:
    Rectangle leftHitbox();
    Rectangle rightHitbox();
    Rectangle topHitbox();
    Rectangle bottomHitbox();
};