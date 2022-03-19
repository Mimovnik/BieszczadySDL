
#ifndef ALIVE_H
#define ALIVE_H

#include "Animation.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "Timer.h"
class Alive : public GameObject {
   public:
    Timer jumpTimer;
    Timer placeTimer;
    Timer digTimer;
    double walkAccel;
    double jumpHeight;

    Animation idle;
    Animation walking;
    Animation jumping;
    Animation falling;

    Alive(RigidBody rigidbody,
          std::vector<std::vector<SDL_Surface*>> heroSurfaceListList,
          double walkAccel, double jumpHeight, double jumpCooldown) {
        printf("Log2:\n");
        this->rb = new RigidBody;
        this->rb->hitbox.position = rigidbody.hitbox.position;
        this->rb->hitbox.width = rigidbody.hitbox.width;
        this->rb->hitbox.height = rigidbody.hitbox.height;
        this->rb->maxSpeed = rigidbody.maxSpeed;

        this->walkAccel = walkAccel;
        this->jumpHeight = jumpHeight;
        this->jumpTimer.setCooldown(jumpCooldown);
        this->placeTimer.setCooldown(0);
        this->digTimer.setCooldown(0);

        idle = Animation(heroSurfaceListList[0], heroSurfaceListList[1], "idle",
                         0.2);
        walking = Animation(heroSurfaceListList[2], heroSurfaceListList[3],
                            "walking", 0.15);
        jumping = Animation(heroSurfaceListList[4], heroSurfaceListList[5],
                            "jumping", 0.15);
        falling = Animation(heroSurfaceListList[6], heroSurfaceListList[7],
                            "falling", 0.1);
        renderer.setActive(&idle);
    }

    void place(GameObject block, Vector mousePos, QuadTree* terrain,
               double realTime);

    void dig(Vector digPos, QuadTree* terrain, double realTime);

    void jump(RigidBody* bases, int basesCount, double realTime);

    void walk(char direction, RigidBody* bases, int basesCount);

    void startAnimation(Animation* animation);
};

#endif