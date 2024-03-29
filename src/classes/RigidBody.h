#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <SDL.h>

#include <vector>

#include "Animation.h"
#include "Rectangle.h"
#include "Timer.h"
#include "Vector.h"

class RigidBody {
   public:
    RigidBody* colliders = nullptr;
    int collidersCount;
    bool collidable;
    Vector velocity;
    Vector acceleration;
    double mass;
    Rectangle hitbox;
    double maxSpeed;

    RigidBody(){};

    RigidBody(const RigidBody& other)
        : velocity(other.velocity),
          acceleration(other.acceleration),
          mass(other.mass),
          hitbox(other.hitbox),
          collidable(other.collidable),
          maxSpeed(other.maxSpeed) {}

    RigidBody(Vector startingPosition, int width, int height,
              bool collidable = true, double maxSpeed = 10) {
        this->hitbox.position = startingPosition;
        this->hitbox.width = width;
        this->hitbox.height = height;
        this->collidable = collidable;
        this->maxSpeed = maxSpeed;
    }

    void move(double gameDelta);

    void collide(double gameDelta);

    Vector futureOffset(double gameDelta);

    Rectangle leftHitbox();
    Rectangle rightHitbox();
    Rectangle topHitbox();
    Rectangle bottomHitbox();
};

#endif