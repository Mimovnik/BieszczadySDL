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
            rb.acceleration += Vector(-moveAccel, 0);
            return;
        }
        if (direction == 'R') {
            rb.acceleration += Vector(moveAccel, 0);
            return;
        }
    }
    // airborne
    if (direction == 'L') {
        rb.acceleration += Vector(-moveAccel * 0.3, 0);
        return;
    }
    if (direction == 'R') {
        rb.acceleration += Vector(moveAccel * 0.3, 0);
        return;
    }
    delete[] othersHitboxes;
}

void Alive::attack(Alive* creature, char direction, double realTime) {
    if (attackTimer.isUp(realTime)) {
        attackTimer.start(realTime);
        const int weaponDamage = 1;
        const int areaW = 64;
        const int areaH = 64;
        const int knockbackStrength = 50;
        Vector weaponDir;
        if(direction == 'L'){
            weaponDir = rb.hitbox.position.addX(-areaW / 2);
        }
        if(direction == 'R'){
            weaponDir = rb.hitbox.position.addX(areaW / 2);
        }
        Rectangle weaponDamageArea(areaW, areaH,
                                   weaponDir);
        if (weaponDamageArea.overlaps(creature->rb.hitbox)) {
            creature->health -= weaponDamage;
            Vector knockback;
            if(direction == 'L'){
                knockback = Vector(-knockbackStrength, 0);
            } else if(direction == 'R'){
                knockback = Vector(knockbackStrength, 0);
            }
            creature->rb.velocity += knockback;
        }
        if (creature->health <= 0) {
            creature->die();
        }
    }
}

void Alive::flyTo(Vector position, double realTime){
    Vector direction((position  - rb.hitbox.position).addY(-32));
    rb.acceleration += Vector::fromAngle(moveAccel, direction.getAngle());
}

void Alive::die(){
    printf("A creature died");
}

void Alive::startAnimation(Animation* animation) { rndr.active = animation; }
