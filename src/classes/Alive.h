#ifndef ALIVE_H
#define ALIVE_H

#include <vector>

#include "Animation.h"
#include "GameObject.h"
#include "QuadTree.h"
#include "RigidBody.h"
#include "Timer.h"
#include "Tool.h"
#include "Weapon.h"
class Alive : public GameObject {
   public:
    bool alive;
    Timer jumpTimer;
    Timer placeDelay;
    Timer digDelay;
    Timer attackFreq;
    double moveAccel;
    double jumpHeight;
    int killCount = 0;

    Weapon* weapon;
    
    Tool* tool;
    
    GameObject* buildingBlock;

    Animation idle;
    Animation walking;
    Animation jumping;
    Animation falling;
    Animation attacking1;
    Animation hurting;
    Animation dying;
    Animation died;

    Alive(RigidBody rb, Weapon weapon, Tool tool,
          std::vector<std::vector<SDL_Surface*>> surfaces, double moveAccel,
          double jumpHeight, double jumpCooldown, double attackFrequency,
          int maxHealth, double idleAnimSpeed, double attack1AnimSpeed,
          double hurtingAnimSpeed, double dyingAnimSpeed,
          double walkAnimSpeed = 0.2, double jumpAnimSpeed = 0.2,
          double fallAnimSpeed = 0.2);

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