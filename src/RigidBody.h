#include <SDL.h>
#include <vector>

#include "Animation.h"
#include "Rectangle.h"
#include "Timer.h"
#include "Vector.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
class RigidBody {
   public:
    Animation active;
    bool drawScaledToHitbox;
    bool collidable;
    Vector velocity;
    Vector acceleration;
    double mass;
    Rectangle hitbox;
    double maxSpeed;

    RigidBody(){};

    RigidBody(Vector startingPosition,
              std::vector<SDL_Surface*> idleSurfaceList, int width, int height,
              bool collidable = true, bool drawScaledToHitbox = true,
              double maxSpeed = 10) {
        this->hitbox.position = startingPosition;
        this->hitbox.width = width;
        this->hitbox.height = height;
        this->collidable = collidable;
        this->drawScaledToHitbox = drawScaledToHitbox;
        this->maxSpeed = maxSpeed;

        this->active = Animation(idleSurfaceList, idleSurfaceList, "idle", 0.2);
    }

    void move(double gameDelta, RigidBody* others,
                           int othersCount);

    void draw(SDL_Surface* screen, Vector offset);

    void collide(RigidBody* others, int othersCount, double gameDelta);

   protected:
    Rectangle leftHitbox();
    Rectangle rightHitbox();
    Rectangle topHitbox();
    Rectangle bottomHitbox();
};

#endif