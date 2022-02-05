#pragma once
#include "RigidBody.h"
class Alive : public RigidBody {
   public:
	Alive(Vector startingPosition, SDL_Surface* surface,int width, int height, double accelerationRate);
    

    void jump(RigidBody another);
};