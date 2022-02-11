#pragma once
#include "QuadTree.h"
#include "RigidBody.h"
#include "Timer.h"
class Alive : public RigidBody {
   public:
    Timer jumpTimer;
    Timer placeTimer;
    Timer digTimer;
    double walkAccel;
    double jumpHeight;

    std::vector<SDL_Surface*> idleLeftSurfaceList;
    std::vector<SDL_Surface*> idleRightSurfaceList;
    std::vector<SDL_Surface*> walkLeftSurfaceList;
    std::vector<SDL_Surface*> walkRightSurfaceList;
    std::vector<SDL_Surface*> jumpLeftSurfaceList;
    std::vector<SDL_Surface*> jumpRightSurfaceList;
    std::vector<SDL_Surface*> fallLeftSurfaceList;
    std::vector<SDL_Surface*> fallRightSurfaceList;

    Alive(Vector startingPosition, std::vector<std::vector<SDL_Surface*>> heroSurfaceListList, int width, int height,
          double maxSpeed, double walkAccel, double jumpHeight,
          double jumpCooldown);

    void place(RigidBody block, Vector mousePos, QuadTree* terrain,
               double realTime);

    void dig(Vector digPos, QuadTree* terrain, double realTime);

    void jump(RigidBody* bases, int basesCount, double realTime);

    void move(char direction, RigidBody* bases, int basesCount);

    void startAnimation(std::vector<SDL_Surface*> surfaceList);
};