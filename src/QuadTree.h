#pragma once
#include <vector>

#include "RigidBody.h"
#define NODE_CAPACITY 1

class QuadTree {
   public:
    Rectangle boundary;

    RigidBody blocks[NODE_CAPACITY];
    int blocksAdded;

    QuadTree* topLeft = nullptr;
    QuadTree* topRight = nullptr;
    QuadTree* botLeft = nullptr;
    QuadTree* botRight = nullptr;

    QuadTree();

    QuadTree(Rectangle boundary) {
        this->boundary = boundary;
        this->blocksAdded = 0;
    }

    bool insert(RigidBody block);

    bool destroy(Vector point);

    std::vector<RigidBody> queryRange(Rectangle range);
    std::vector<RigidBody> queryRange(Vector containingPoint);

    void subdivide(RigidBody* blocks);
};