#include "Alive.h"

#include <math.h>

#include "RigidBody.h"
#include "Vector.h"

void Alive::place(GameObject block, Vector mousePos, QuadTree* terrain,
                  double realTime) {
    Vector blockPos;
    blockPos.x = round(mousePos.x) - ((int)round(mousePos.x) % 64) + 32;
    blockPos.y = round(mousePos.y) - ((int)round(mousePos.y) % 64) - 32;
    block.rb.hitbox.position = blockPos;
    if (!this->rb.hitbox.overlaps(block.rb.hitbox)) {
        if (terrain->queryRange(block.rb.hitbox).size() == 0) {
            if (placeTimer.isUp(realTime)) {
                placeTimer.start(realTime);

                terrain->insert(block);
            }
        }
    }
}

void Alive::dig(Vector digPos, QuadTree* terrain, double realTime) {
    // if (terrain->queryRange(Rectangle(64, 64, digPos)).size() > 0) {
    if (digTimer.isUp(realTime)) {
        digTimer.start(realTime);

        terrain->destroy(digPos);
    }
    // }
}

void Alive::jump(RigidBody* bases, int basesCount, double realTime) {
    Vector below = Vector(0, 1);
    Rectangle* othersHitboxes = new Rectangle[basesCount];
    for (int i = 0; i < basesCount; i++) {
        othersHitboxes[i] = bases[i].hitbox;
    }
    if (rb.bottomHitbox().translate(below).overlapsAny(othersHitboxes,
                                                       basesCount)) {
        if (jumpTimer.isUp(realTime)) {
            jumpTimer.start(realTime);

            rb.velocity.y = -jumpHeight;
        }
    }
    delete[] othersHitboxes;
}

void Alive::walk(char direction, RigidBody* bases, int basesCount) {
    Vector below = Vector(0, 1);

    Rectangle* othersHitboxes = new Rectangle[basesCount];
    for (int i = 0; i < basesCount; i++) {
        othersHitboxes[i] = bases[i].hitbox;
    }
    if (rb.bottomHitbox().translate(below).overlapsAny(othersHitboxes,
                                                       basesCount)) {
        if (direction == 'L') {
            rb.acceleration = rb.acceleration.add(Vector(-walkAccel, 0));
            return;
        }
        if (direction == 'R') {
            rb.acceleration = rb.acceleration.add(Vector(walkAccel, 0));
            return;
        }
    }
    // airborne
    if (direction == 'L') {
        rb.acceleration = rb.acceleration.add(Vector(-walkAccel * 0.3, 0));
        return;
    }
    if (direction == 'R') {
        rb.acceleration = rb.acceleration.add(Vector(walkAccel * 0.3, 0));
        return;
    }
    delete[] othersHitboxes;
}

void Alive::startAnimation(Animation* animation) { rndr.active = animation; }
