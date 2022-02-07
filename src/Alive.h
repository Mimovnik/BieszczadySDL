#pragma once
#include "RigidBody.h"
#include "Timer.h"
class Alive : public RigidBody {
   public:
    Timer jumpTimer;
    double walkAccel;
    double jumpHeight;

	Alive(Vector startingPosition, SDL_Surface* surface,int width, int height, double maxSpeed, double walkAccel, double jumpHeight, double jumpCooldown);

    void jump(RigidBody another, double realTime);

    void move(char direction, RigidBody another);
};