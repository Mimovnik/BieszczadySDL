#include "Alive.h"

#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(Vector startingPosition, SDL_Surface* surface, int width,
             int height, double maxSpeed, double walkAccel, double jumpHeight,
             double jumpCooldown) {
    this->hitbox.position = startingPosition;
    this->surface = surface;
    this->hitbox.width = width;
    this->hitbox.height = height;
    this->maxSpeed = maxSpeed;
    this->walkAccel = walkAccel;
    this->jumpHeight = jumpHeight;
    this->jumpTimer.coolDown = jumpCooldown;
    this->placeTimer.coolDown = 0.1;
}

void Alive::place(RigidBody block, Vector blockPos, QuadTree* terrain,
                  double realTime) {
    block.hitbox.position = blockPos;
    if (placeTimer.isUp(realTime)) {
        placeTimer.start(realTime);

        terrain->insert(block);
    }
}

void Alive::jump(RigidBody base, double realTime) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(base.hitbox)) {
        if (jumpTimer.isUp(realTime)) {
            jumpTimer.start(realTime);

            velocity.y = -jumpHeight;
        }
    }
}

void Alive::jump(RigidBody* bases, int basesCount, double realTime) {
    Vector below = Vector(0, 1);
    Rectangle* othersHitboxes = new Rectangle[basesCount];
    for (int i = 0; i < basesCount; i++) {
        othersHitboxes[i] = bases[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlaps(othersHitboxes, basesCount)) {
        if (jumpTimer.isUp(realTime)) {
            jumpTimer.start(realTime);

            velocity.y = -jumpHeight;
        }
    }
    delete[] othersHitboxes;
}

void Alive::move(char direction, RigidBody base) {
    Vector below = Vector(0, 1);
    if (bottomHitbox().translate(below).overlaps(base.hitbox)) {
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

void Alive::move(char direction, RigidBody* bases, int basesCount) {
    Vector below = Vector(0, 1);

    Rectangle* othersHitboxes = new Rectangle[basesCount];
    for (int i = 0; i < basesCount; i++) {
        othersHitboxes[i] = bases[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlaps(othersHitboxes, basesCount)) {
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
    delete[] othersHitboxes;
}
