#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"
#include "RigidBody.h"

class GameObject {
   public:
    Renderer renderer;
    RigidBody* rb = nullptr;

    GameObject() {}

    GameObject(Renderer renderer) { this->renderer = renderer; }

    GameObject(Renderer renderer, RigidBody* rigidbody) {
        this->renderer = renderer;
        this->rb = new RigidBody;
        this->rb = rigidbody;
    }

    Vector position(){
        return rb->hitbox.position;
    }

    ~GameObject() {
        if (rb != nullptr) {
            delete rb;
        }
    }
};

#endif