#include "Alive.h"

#include <math.h>

#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(Vector startingPosition, std::vector<std::vector<SDL_Surface*>> heroSurfaceListList, int width,
             int height, double maxSpeed, double walkAccel, double jumpHeight,
             double jumpCooldown) {
    this->hitbox.position = startingPosition;
    this->hitbox.width = width;
    this->hitbox.height = height;

    this->maxSpeed = maxSpeed;
    this->walkAccel = walkAccel;
    this->jumpHeight = jumpHeight;
    this->jumpTimer.coolDown = jumpCooldown;

    this->placeTimer.coolDown = 0.2;
    this->digTimer.coolDown = 0.1;

    this->idleLeftSurfaceList = heroSurfaceListList[0];
    this->idleRightSurfaceList = heroSurfaceListList[1];
    this->walkLeftSurfaceList = heroSurfaceListList[2];
    this->walkRightSurfaceList = heroSurfaceListList[3];
    this->jumpLeftSurfaceList = heroSurfaceListList[4];
    this->jumpRightSurfaceList = heroSurfaceListList[5];
    this->fallLeftSurfaceList = heroSurfaceListList[6];
    this->fallRightSurfaceList = heroSurfaceListList[7];
    this->currentSurfaceList = idleRightSurfaceList;
    this->animation.coolDown = 0.15;
    this->currentSurfaceIndex = 0;
}

void Alive::place(RigidBody block, Vector mousePos, QuadTree* terrain,
                  double realTime) {
    Vector blockPos;
    blockPos.x = round(mousePos.x) - ((int)round(mousePos.x) % 64) + 32;
    blockPos.y = round(mousePos.y) - ((int)round(mousePos.y) % 64) - 32;
    block.hitbox.position = blockPos;
    if(!this->hitbox.overlaps(block.hitbox)){
    if (terrain->queryRange(block.hitbox).size() == 0) {
        if (placeTimer.isUp(realTime)) {
            placeTimer.start(realTime);

            terrain->insert(block);
        }
    }
    }
}

void Alive::dig(Vector digPos, QuadTree* terrain, double realTime) {
    if (terrain->queryRange(digPos).size() > 0) {
        if (digTimer.isUp(realTime)) {
            digTimer.start(realTime);

            terrain->destroy(digPos);
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

void Alive::move(char direction, RigidBody* bases, int basesCount) {
    Vector below = Vector(0, 1);

    Rectangle* othersHitboxes = new Rectangle[basesCount];
    for (int i = 0; i < basesCount; i++) {
        othersHitboxes[i] = bases[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlaps(othersHitboxes, basesCount)) {
        if (direction == 'L') {
            acceleration = acceleration.add(Vector(-walkAccel, 0));
            return;
        }
        if (direction == 'R') {
            acceleration = acceleration.add(Vector(walkAccel, 0));
            return;
        }
    }
    if (direction == 'L') {
        acceleration = acceleration.add(Vector(-walkAccel * 0.1, 0));
        return;
    }
    if (direction == 'R') {
        acceleration = acceleration.add(Vector(walkAccel * 0.1, 0));
        return;
    }
    delete[] othersHitboxes;
}

    void Alive::startAnimation(std::vector<SDL_Surface*> surfaceList){
        if(currentSurfaceList != surfaceList){
            currentSurfaceList = surfaceList;
            currentSurfaceIndex = 0;
        }
    }

