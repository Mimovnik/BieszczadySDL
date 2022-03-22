#ifndef ALIVE_H
#define ALIVE_H

#include <vector>

#include "Animation.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "Timer.h"
class Alive : public GameObject {
   public:
    bool alive;
    int health;
    const int maxHealth;
    Timer jumpTimer;
    Timer placeTimer;
    Timer digTimer;
    Timer attackTimer;
    double moveAccel;
    double jumpHeight;
    int killCount = 0;

    Animation idle;
    Animation walking;
    Animation jumping;
    Animation falling;

    Alive(RigidBody rb, std::vector<std::vector<SDL_Surface*>> surfaces,
          double moveAccel, double jumpHeight, double jumpCooldown,
          int maxHealth_, double idleAnimSpeed = 0.2, double walkAnimSpeed = 0.2,
          double jumpAnimSpeed = 0.2, double fallAnimSpeed = 0.2)
        : maxHealth(maxHealth_) {
        this->rb = rb;
        this->health = maxHealth;
        this->moveAccel = moveAccel;
        this->jumpHeight = jumpHeight;
        this->jumpTimer.setCooldown(jumpCooldown);
        this->alive = true;

        this->placeTimer.setCooldown(0);
        this->digTimer.setCooldown(0);
        this->attackTimer.setCooldown(0.5);

        if (!surfaces[0].empty() && !surfaces[1].empty())
            idle = Animation(surfaces[0], surfaces[1], "idle", idleAnimSpeed);

        if (!surfaces[2].empty() && !surfaces[3].empty())
            walking = Animation(surfaces[2], surfaces[3], "walking", walkAnimSpeed);

        if (!surfaces[4].empty() && !surfaces[5].empty())
            jumping = Animation(surfaces[4], surfaces[5], "jumping", jumpAnimSpeed);

        if (!surfaces[6].empty() && !surfaces[7].empty())
            falling = Animation(surfaces[6], surfaces[7], "falling", fallAnimSpeed);

        rndr.active = &idle;
    }

    void place(GameObject block, Vector mousePos, QuadTree* terrain,
               double realTime);

    void dig(Vector digPos, QuadTree* terrain, double realTime);

    void jump(double realTime);

    void walk(char direction);

    void flyTo(Vector position, double realTime);

    void attack(Alive* creature, char direction, double realTime);

    void startAnimation(Animation* animation);

    void die();

    bool isAlive();
};

#endif