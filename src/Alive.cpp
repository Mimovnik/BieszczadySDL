#include "Alive.h"

#include <math.h>

#include "RigidBody.h"
#include "Vector.h"
#include "settings.h"

void Alive::place(GameObject block, Vector mousePos, QuadTree* terrain,
                  double realTime) {
    Vector blockPos;
    blockPos.x = round(mousePos.x) - ((int)round(mousePos.x) % BLOCK_WIDTH) +
                 (BLOCK_WIDTH / 2);
    blockPos.y = round(mousePos.y) - ((int)round(mousePos.y) % BLOCK_HEIGHT) -
                 (BLOCK_HEIGHT / 2);
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
    if (digTimer.isUp(realTime)) {
        digTimer.start(realTime);

        terrain->destroy(digPos);
    }
}

void Alive::jump(double realTime) {
    Vector below = Vector(0, 1);
    Rectangle* othersHitboxes = new Rectangle[rb.collidersCount];
    for (int i = 0; i < rb.collidersCount; i++) {
        othersHitboxes[i] = rb.colliders[i].hitbox;
    }
    if (rb.bottomHitbox().translate(below).overlapsAny(othersHitboxes,
                                                       rb.collidersCount)) {
        if (jumpTimer.isUp(realTime)) {
            jumpTimer.start(realTime);

            rb.velocity.y = -jumpHeight;
        }
    }
    delete[] othersHitboxes;
}

void Alive::walk(char direction) {
    Vector below = Vector(0, 1);

    Rectangle* othersHitboxes = new Rectangle[rb.collidersCount];
    for (int i = 0; i < rb.collidersCount; i++) {
        othersHitboxes[i] = rb.colliders[i].hitbox;
    }
    if (rb.bottomHitbox().translate(below).overlapsAny(othersHitboxes,
                                                       rb.collidersCount)) {
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

void Alive::attack(Alive* creature, double realTime) {
    if (attackTimer.isUp(realTime)) {
        attackTimer.start(realTime);
        const int weaponDamage = 1;
        const int areaW = 30;
        const int areaH = 10;
        Rectangle weaponDamageArea(areaW, areaH,
                                   rb.hitbox.position.addX(areaW / 2));
        if (weaponDamageArea.overlaps(creature->rb.hitbox)) {
            creature->health -= weaponDamage;
        }
        if (creature->health <= 0) {
            creature->jump(realTime);
        }
    }
}

void Alive::startAnimation(Animation* animation) { rndr.active = animation; }
