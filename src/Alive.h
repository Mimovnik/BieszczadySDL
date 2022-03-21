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
    int health;
    const int maxHealth;
    Timer jumpTimer;
    Timer placeTimer;
    Timer digTimer;
    Timer attackTimer;
    double moveAccel;
    double jumpHeight;

    Animation idle;
    Animation walking;
    Animation jumping;
    Animation falling;

    Alive(RigidBody rb,
          std::vector<std::vector<SDL_Surface*>> heroSurfaceListList, double moveAccel, double jumpHeight,
          double jumpCooldown, int maxHealth_): maxHealth(maxHealth_) {
        this->rb = rb;
        this->health = maxHealth;
        this->moveAccel = moveAccel;
        this->jumpHeight = jumpHeight;
        this->jumpTimer.setCooldown(jumpCooldown);

        this->placeTimer.setCooldown(0);
        this->digTimer.setCooldown(0);
        this->attackTimer.setCooldown(0.5);

        idle = Animation(heroSurfaceListList[0], heroSurfaceListList[1], "idle",
                         0.2);
        walking = Animation(heroSurfaceListList[2], heroSurfaceListList[3],
                            "walking", 0.15);
        jumping = Animation(heroSurfaceListList[4], heroSurfaceListList[5],
                            "jumping", 0.15);
        falling = Animation(heroSurfaceListList[6], heroSurfaceListList[7],
                            "falling", 0.1);
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
};

#endif