#pragma once
#include <vector>

#include "RigidBody.h"
#define NODE_CAPACITY 4

class QuadTree {
   public:
    Rectangle boundary;

    Vector points[NODE_CAPACITY];
    int pointsAdded;

    QuadTree* topLeft = nullptr;
    QuadTree* topRight = nullptr;
    QuadTree* botLeft = nullptr;
    QuadTree* botRight = nullptr;

    QuadTree(Rectangle boundary) {
        this->boundary = boundary;
        this->pointsAdded = 0;
    }

    bool insert(Vector point);

    std::vector<Vector> queryRange(Rectangle range);

    void subdivide();
};