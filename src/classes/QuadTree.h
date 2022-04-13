#ifndef QUADTREE_H
#define QUADTREE_H
#include <vector>

#include "GameObject.h"
#include "../functions/settings.h"

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


    QuadTree(Rectangle boundary) {
        this->boundary = boundary;
        this->blocksAdded = 0;
    }

    ~QuadTree();

    bool insert(GameObject block);

    bool dig(Vector point, int efficiency);

    std::vector<GameObject> queryRange(Rectangle range);
    

    void subdivide(GameObject* blocks);
};
#endif