#pragma once
#include "RigidBody.h"
#include "Timer.h"
#include "QuadTree.h"
class Alive : public RigidBody {
   public:
    Timer jumpTimer;
    Timer placeTimer;
    Timer digTimer;
    double walkAccel;
    double jumpHeight;

	Alive(Vector startingPosition, SDL_Surface* surface,int width, int height, double maxSpeed, double walkAccel, double jumpHeight, double jumpCooldown);

    void place(RigidBody block, Vector mousePos, QuadTree* terrain, double realTime);

    void dig(Vector digPos, QuadTree* terrain, double realTime);

    void jump(RigidBody* bases, int basesCount, double realTime);

    void move(char direction, RigidBody* bases, int basesCount);
};