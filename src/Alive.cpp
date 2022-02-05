#include "Alive.h"

#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(Vector startingPosition, SDL_Surface* surface, int width,
             int height, double accelerationRate) {
    this->hitbox.position = startingPosition;
    this->surface = surface;
    this->hitbox.width = width;
    this->hitbox.height = height;
    this->accelerationRate = accelerationRate;
}

void Alive::jump(RigidBody another) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
        acceleration.y = -100;
    }
}

void Alive::move(char direction, RigidBody another) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
        if (direction == 'R') {
            acceleration = acceleration.add(Vector(-accelerationRate, 0));
            return;
        }
        if (direction == 'L') {
            acceleration = acceleration.add(Vector(accelerationRate, 0));
            return;
        }
    }
}