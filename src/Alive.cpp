#include "Alive.h"

#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(Vector startingPosition, SDL_Surface* surface, int width,
             int height, double maxSpeed, double walkAccel, double jumpHeight, double jumpCooldown) {
    this->hitbox.position = startingPosition;
    this->surface = surface;
    this->hitbox.width = width;
    this->hitbox.height = height;
    this->maxSpeed = maxSpeed;
    this->walkAccel = walkAccel;
    this->jumpHeight = jumpHeight;
    this->jumpTimer.coolDown = jumpCooldown;
}

void Alive::jump(RigidBody another, double realTime) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
        if (jumpTimer.isUp(realTime)) {
            jumpTimer.start(realTime);

            velocity.y = -jumpHeight;
        }
    }
}

void Alive::move(char direction, RigidBody another) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
        if (direction == 'R') {
            acceleration = acceleration.add(Vector(-walkAccel, 0));
            return;
        }
        if (direction == 'L') {
            acceleration = acceleration.add(Vector(walkAccel, 0));
            return;
        }
    }
    if (direction == 'R') {
        acceleration = acceleration.add(Vector(-walkAccel * 0.1, 0));
        return;
    }
    if (direction == 'L') {
        acceleration = acceleration.add(Vector(walkAccel * 0.1, 0));
        return;
    }
}
