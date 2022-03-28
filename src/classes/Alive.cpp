#include "Alive.h"

#include <math.h>

#include "RigidBody.h"
#include "Vector.h"
#include "../functions/settings.h"

Alive::Alive(RigidBody rb, Weapon weapon_,
          std::vector<std::vector<SDL_Surface*>> surfaces, double moveAccel,
          double jumpHeight, double jumpCooldown, double attackFrequency,
          int maxHealth_, double idleAnimSpeed, double attack1AnimSpeed,
          double hurtingAnimSpeed, double dyingAnimSpeed,
          double walkAnimSpeed, double jumpAnimSpeed,
          double fallAnimSpeed)
        : maxHealth(maxHealth_), weapon(weapon_) {
        this->rb = rb;
        this->health = maxHealth;
        this->moveAccel = moveAccel;
        this->jumpHeight = jumpHeight;
        this->jumpTimer.setCooldown(jumpCooldown);
        this->alive = true;

        if (!surfaces[0].empty() && !surfaces[1].empty())
            idle = Animation(surfaces[0], surfaces[1], "idle", idleAnimSpeed);

        if (!surfaces[2].empty() && !surfaces[3].empty())
            walking =
                Animation(surfaces[2], surfaces[3], "walking", walkAnimSpeed);

        if (!surfaces[4].empty() && !surfaces[5].empty())
            jumping =
                Animation(surfaces[4], surfaces[5], "jumping", jumpAnimSpeed);

        if (!surfaces[6].empty() && !surfaces[7].empty())
            falling =
                Animation(surfaces[6], surfaces[7], "falling", fallAnimSpeed);

        if (!surfaces[8].empty() && !surfaces[9].empty())
            attacking1 = Animation(surfaces[8], surfaces[9], "attack1",
                                   attack1AnimSpeed);

        if (!surfaces[10].empty() && !surfaces[11].empty())
            hurting = Animation(surfaces[10], surfaces[11], "hurting",
                                hurtingAnimSpeed);

        if (!surfaces[12].empty() && !surfaces[13].empty())
            dying =
                Animation(surfaces[12], surfaces[13], "dying", dyingAnimSpeed);

        if (!surfaces[14].empty())
            died =
                Animation(surfaces[14], surfaces[14], "died", 100);

        rndr.active = &idle;

        this->placeDelay.setCooldown(0);
        this->digDelay.setCooldown(0);
        this->attackFreq.setCooldown(attackFrequency);
    }

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
            weaponDir.x += -weapon.damageArea.width / 4;
            attacking1.changeSide('L');
        }
        if (direction == 'R') {
            weaponDir.x += weapon.damageArea.width / 4;
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
    rb.acceleration.y -= 10;
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
