#ifndef ALIVE_H
#define ALIVE_H

#include <vector>

#include "Animation.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "Timer.h"
#include "Weapon.h"
class Alive : public GameObject {
   public:
    bool alive;
    int health;
    const int maxHealth;
    Timer jumpTimer;
    Timer placeDelay;
    Timer digDelay;
    Timer attackFreq;
    double moveAccel;
    double jumpHeight;
    int killCount = 0;

    Weapon weapon;

    Animation idle;
    Animation walking;
    Animation jumping;
    Animation falling;
    Animation attacking1;
    Timer attackEnd;

    Alive(RigidBody rb, Weapon weapon_,
          std::vector<std::vector<SDL_Surface*>> surfaces, double moveAccel,
          double jumpHeight, double jumpCooldown, double attackFrequency, int maxHealth_,
          double idleAnimSpeed = 0.2, double attack1AnimSpeed = 0.1,
          double walkAnimSpeed = 0.2, double jumpAnimSpeed = 0.2,
          double fallAnimSpeed = 0.2)
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

        rndr.active = &idle;

        
        this->placeDelay.setCooldown(0);
        this->digDelay.setCooldown(0);
        this->attackFreq.setCooldown(attackFrequency);
        this->attackEnd.setCooldown(attack1AnimSpeed * attacking1.rightSurfaceList.size());
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