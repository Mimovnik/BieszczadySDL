#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>

#include "RigidBody.h"
#include "settings.h"
#include "GameObject.h"


#define NODE_CAPACITY 4
class QuadTree {
   public:
    Rectangle boundary;

    GameObject blocks[NODE_CAPACITY];
    int blocksAdded;

    QuadTree* topLeft = nullptr;
    QuadTree* topRight = nullptr;
    QuadTree* botLeft = nullptr;
    QuadTree* botRight = nullptr;

    QuadTree(){};

    QuadTree(Rectangle boundary) {
        this->boundary = boundary;
        this->blocksAdded = 0;
    }

    bool insert(GameObject block);

    bool destroy(Vector point);

    std::vector<GameObject> queryRange(Rectangle range);
    std::vector<GameObject> queryRange(Vector containingPoint);

    void subdivide(GameObject* blocks);
};
#endif