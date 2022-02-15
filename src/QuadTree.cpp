#include "QuadTree.h"

bool QuadTree::insert(RigidBody block) {
    if (!boundary.contains(block.hitbox.position)) {
        return false;
    }
    if (blocksAdded < NODE_CAPACITY && topLeft == nullptr) {
        blocks[blocksAdded] = block;
        blocksAdded++;
        return true;
    }
    if (topLeft == nullptr) {
        subdivide(blocks);
    }
    if (topLeft->insert(block)) return true;
    if (topRight->insert(block)) return true;
    if (botLeft->insert(block)) return true;
    if (botRight->insert(block)) return true;

    return false;
}

bool QuadTree::destroy(Vector point) {
    if (!boundary.overlaps(Rectangle(BLOCK_WIDTH, BLOCK_HEIGHT, point))) {
        return false;
    }
    if (topLeft != nullptr) {
        topLeft->destroy(point);
        topRight->destroy(point);
        botLeft->destroy(point);
        botRight->destroy(point);
    }

for (int i = 0; i < blocksAdded; i++) {
        if (blocks[i].hitbox.contains(point)) {
           blocks[i]= RigidBody();
        }
    }

    return true;
}

std::vector<RigidBody> QuadTree::queryRange(Rectangle range) {
    std::vector<RigidBody> blocksInRange;
    if (!boundary.overlaps(range)) {
        return blocksInRange;
    }

    if (topLeft != nullptr) {
        std::vector<RigidBody> topLeftInRange = topLeft->queryRange(range);
        blocksInRange.insert(blocksInRange.end(), topLeftInRange.begin(),
                             topLeftInRange.end());

        std::vector<RigidBody> topRightInRange = topRight->queryRange(range);
        blocksInRange.insert(blocksInRange.end(), topRightInRange.begin(),
                             topRightInRange.end());

        std::vector<RigidBody> botLeftInRange = botLeft->queryRange(range);
        blocksInRange.insert(blocksInRange.end(), botLeftInRange.begin(),
                             botLeftInRange.end());

        std::vector<RigidBody> botRightInRange = botRight->queryRange(range);
        blocksInRange.insert(blocksInRange.end(), botRightInRange.begin(),
                             botRightInRange.end());
    }

    for (int i = 0; i < blocksAdded; i++) {
        if (range.overlaps(blocks[i].hitbox)) {
            blocksInRange.push_back(blocks[i]);
        }
    }

    return blocksInRange;
}

std::vector<RigidBody> QuadTree::queryRange(Vector containingPoint) {
    std::vector<RigidBody> blocksContainingPoint;
    if (!boundary.contains(containingPoint)) {
        return blocksContainingPoint;
    }

    if (topLeft != nullptr) {
        std::vector<RigidBody> topLeftInRange =
            topLeft->queryRange(containingPoint);
        blocksContainingPoint.insert(blocksContainingPoint.end(),
                                     topLeftInRange.begin(),
                                     topLeftInRange.end());

        std::vector<RigidBody> topRightInRange =
            topRight->queryRange(containingPoint);
        blocksContainingPoint.insert(blocksContainingPoint.end(),
                                     topRightInRange.begin(),
                                     topRightInRange.end());

        std::vector<RigidBody> botLeftInRange =
            botLeft->queryRange(containingPoint);
        blocksContainingPoint.insert(blocksContainingPoint.end(),
                                     botLeftInRange.begin(),
                                     botLeftInRange.end());

        std::vector<RigidBody> botRightInRange =
            botRight->queryRange(containingPoint);
        blocksContainingPoint.insert(blocksContainingPoint.end(),
                                     botRightInRange.begin(),
                                     botRightInRange.end());
    }

    for (int i = 0; i < blocksAdded; i++) {
        if (blocks[i].hitbox.contains(containingPoint)) {
            blocksContainingPoint.push_back(blocks[i]);
        }
    }

    return blocksContainingPoint;
}

void QuadTree::subdivide(RigidBody* blocks) {
    Vector topLeftPosition = boundary.position.add(
        Vector(-boundary.width / 4, -boundary.height / 4));
    Rectangle topLeftBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topLeftPosition);
    topLeft = new QuadTree(topLeftBoundary);

    Vector topRightPosition =
        boundary.position.add(Vector(boundary.width / 4, -boundary.height / 4));
    Rectangle topRightBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topRightPosition);
    topRight = new QuadTree(topRightBoundary);

    Vector botLeftPosition =
        boundary.position.add(Vector(-boundary.width / 4, boundary.height / 4));
    Rectangle botLeftBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, botLeftPosition);
    botLeft = new QuadTree(botLeftBoundary);

    Vector botRightPosition =
        boundary.position.add(Vector(boundary.width / 4, boundary.height / 4));
    Rectangle botRightBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, botRightPosition);
    botRight = new QuadTree(botRightBoundary);

    for (int i = 0; i < NODE_CAPACITY; i++) {
        insert(blocks[i]);
    }
}
