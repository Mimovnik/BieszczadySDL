#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"
#include "RigidBody.h"

class GameObject {
   public:
    Renderer rndr;
    RigidBody rb;

    GameObject() {}

    GameObject(Renderer rndr, RigidBody rb) {
        this->rndr = rndr;
        this->rb = rb;
    }
};

#endif