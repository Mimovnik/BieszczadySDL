#include "Alive.h"

#include <math.h>

#include "../functions/settings.h"
#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(RigidBody rb, Weapon weapon, Tool tool,
             std::vector<std::vector<SDL_Surface*>> surfaces, double moveAccel,
             double jumpHeight, double jumpCooldown, double attackFrequency,
             int maxHealth, double idleAnimFreq, double attack1AnimFreq,
             double attackUpAnimFreq, double hurtingAnimFreq,
             double dyingAnimFreq, double walkAnimFreq, double jumpAnimFreq,
             double fallAnimFreq) {
    this->maxHealth = maxHealth;
    this->weapon = new Weapon(weapon);
    this->tool = new Tool(tool);
    this->rb = rb;
    this->health = maxHealth;
    this->moveAccel = moveAccel;
    this->jumpHeight = jumpHeight;
    this->jumpTimer.setCooldown(jumpCooldown);
    this->alive = true;
    this->actionCursor = Vector::ZERO;
    this->faceDirection = 'R';

    if (!surfaces[0].empty() && !surfaces[1].empty())
        idle = Animation(surfaces[0], surfaces[1], "idle", idleAnimFreq);

    if (!surfaces[2].empty() && !surfaces[3].empty())
        walking = Animation(surfaces[2], surfaces[3], "walking", walkAnimFreq);

    if (!surfaces[4].empty() && !surfaces[5].empty())
        jumping = Animation(surfaces[4], surfaces[5], "jumping", jumpAnimFreq);

    if (!surfaces[6].empty() && !surfaces[7].empty())
        falling = Animation(surfaces[6], surfaces[7], "falling", fallAnimFreq);

    if (!surfaces[8].empty() && !surfaces[9].empty())
        attacking1 =
            Animation(surfaces[8], surfaces[9], "attack1", attack1AnimFreq);

    if (!surfaces[10].empty() && !surfaces[11].empty()) {
        attackingUp =
            Animation(surfaces[10], surfaces[11], "attackUp", attackUpAnimFreq);
    }

    if (!surfaces[12].empty() && !surfaces[13].empty())
        attackingDown = Animation(surfaces[12], surfaces[13], "attackDown",
                                  attack1AnimFreq);

    if (!surfaces[14].empty() && !surfaces[15].empty())
        hurting =
            Animation(surfaces[14], surfaces[15], "hurting", hurtingAnimFreq);

    if (!surfaces[16].empty() && !surfaces[17].empty())
        dying = Animation(surfaces[16], surfaces[17], "dying", dyingAnimFreq);

    if (!surfaces[18].empty())
        died = Animation(surfaces[18], surfaces[19], "died", 100);

    rndr.active = &idle;

    this->placeDelay.setCooldown(0);
    this->digDelay.setCooldown(0.5);
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

        terrain->dig(digPos, tool->efficiency);
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

void Alive::attack1(Alive* creature, double realTime) {
    if (attackFreq.isUp(realTime) && !hurting.isRunning()) {
        attackFreq.start(realTime);

        Vector weaponDir = rb.hitbox.position;

        if (!attacking1.rightSurfaceList.empty()) {
            if (faceDirection == 'U') {
                startAnimation(&attackingUp);
            } else if (faceDirection == 'D') {
                startAnimation(&attackingDown);
            } else {
                startAnimation(&attacking1);
            }
        }

        if (faceDirection == 'L') {
            weaponDir.x += -weapon->hitArea.width / 4;
            attacking1.changeSide('L');
        } else if (faceDirection == 'R') {
            weaponDir.x += weapon->hitArea.width / 4;
            attacking1.changeSide('R');
        } else if (faceDirection == 'D') {
            weaponDir.y += weapon->hitArea.height / 4;
        } else if (faceDirection == 'U') {
            weaponDir.y += -weapon->hitArea.height / 4;
        } else {
            throw "There is no such direction of attack";
        }
        weapon->hitArea.position = weaponDir;
        Vector knockback;
        if (faceDirection == 'L') {
            knockback = Vector(-weapon->knockback, 0);
            creature->hurting.changeSide('R');
        } else if (faceDirection == 'R') {
            knockback = Vector(weapon->knockback, 0);
            creature->hurting.changeSide('L');
        } else if (faceDirection == 'D') {
            knockback = Vector(0, weapon->knockback);
        } else if (faceDirection == 'U') {
            knockback = Vector(0, -weapon->knockback);
        }
        rb.velocity -= knockback.rescale(0.03);
        if (weapon->hitArea.overlaps(creature->rb.hitbox)) {
            if (creature->isAlive())
                creature->startAnimation(&creature->hurting);
            creature->health -= weapon->damage;

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
    alive = false;
    startAnimation(&dying);
}

bool Alive::isAlive() { return alive; }

void Alive::startAnimation(Animation* animation) {
    if (rndr.active != animation) {
        if (rndr.active != nullptr) rndr.active->stop();
        animation->restart();
    }
    rndr.active = animation;
}
