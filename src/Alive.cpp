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
            if (placeDelay.isUp(realTime)) {
                placeDelay.start(realTime);

                terrain->insert(block);
            }
        }
    }
}

void Alive::dig(Vector digPos, QuadTree* terrain, double realTime) {
    if (digDelay.isUp(realTime)) {
        digDelay.start(realTime);

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
    if (attackFreq.isUp(realTime) && !hurting.isRunning()) {
        attackFreq.start(realTime);
        printf("Attack incoming at %.1lf\n", realTime);

        Vector weaponDir = rb.hitbox.position;

        if (!attacking1.rightSurfaceList.empty()) {
            startAnimation(&attacking1);
        }

        if (direction == 'L') {
            weaponDir.x += -weapon.damageArea.width / 2;
            attacking1.changeSide('L');
        }
        if (direction == 'R') {
            weaponDir.x += weapon.damageArea.width / 2;
            attacking1.changeSide('R');
        }
        weapon.damageArea.position = weaponDir;
        if (weapon.damageArea.overlaps(creature->rb.hitbox)) {
            if (creature->isAlive())
                creature->startAnimation(&creature->hurting);
            creature->health -= weapon.damage;

            Vector knockback;
            if (direction == 'L') {
                knockback = Vector(-weapon.knockback, 0);
                creature->hurting.changeSide('L');
            } else if (direction == 'R') {
                knockback = Vector(weapon.knockback, 0);
                creature->hurting.changeSide('R');
            }
            creature->rb.velocity += knockback;
        }
        if (creature->health <= 0 && creature->isAlive()) {
            killCount++;
            creature->die();
        }
    }
}

void Alive::flyTo(Vector position, double realTime) {
    Vector direction((position - rb.hitbox.position).addY(-32));
    rb.acceleration += Vector::fromAngle(moveAccel, direction.getAngle());
}

void Alive::die() {
    printf("A creature died");
    alive = false;
    startAnimation(&dying);
}

bool Alive::isAlive() { return alive; }

void Alive::startAnimation(Animation* animation) {
    if (rndr.active != animation) {
        rndr.active->stop();
        animation->restart();
    }
    rndr.active = animation;
}
