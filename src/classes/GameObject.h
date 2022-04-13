#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"
#include "RigidBody.h"

class GameObject {
   public:
    Renderer rndr;
    RigidBody rb;
    int health;
    int maxHealth;

    GameObject(){}

    GameObject(Renderer rndr, RigidBody rb, int maxHealth){
        this->rndr = rndr;
        this->rb = rb;
        this->maxHealth = maxHealth;
        this->health = maxHealth;
    }

    Vector getPosition() const { return rb.hitbox.position; }
};

#endif