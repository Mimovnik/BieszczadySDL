#include "QuadTree.h"

bool QuadTree::insert(Vector point) {
    if (!boundary.contains(point)) {
        return false;
    }
    if (pointsAdded < NODE_CAPACITY && topLeft == nullptr) {
        points[pointsAdded] = point;
        pointsAdded++;
        return true;
    }
    if (topLeft == nullptr) {
        subdivide();
    }
    if (topLeft->insert(point)) return true;
    if (topRight->insert(point)) return true;
    if (botLeft->insert(point)) return true;
    if (botRight->insert(point)) return true;

    return false;
}

std::vector<Vector> QuadTree::queryRange(Rectangle range) {
    std::vector<Vector> pointsInRange;
    if (!boundary.overlaps(range)) {
        return pointsInRange;
    }

    for (int i = 0; i < pointsAdded; i++) {
        if (range.contains(points[i])) {
            pointsInRange.push_back(points[i]);
        }
    }
    if (topLeft = nullptr) {
        return pointsInRange;
    }

    std::vector<Vector> topLeftInRange = topLeft->queryRange(range);
    pointsInRange.insert(pointsInRange.end(), topLeftInRange.begin(),
                         topLeftInRange.end());

    std::vector<Vector> topRightInRange = topRight->queryRange(range);
    pointsInRange.insert(pointsInRange.end(), topRightInRange.begin(),
                         topRightInRange.end());

    std::vector<Vector> botLeftInRange = botLeft->queryRange(range);
    pointsInRange.insert(pointsInRange.end(), botLeftInRange.begin(),
                         botLeftInRange.end());

    std::vector<Vector> botRightInRange = botRight->queryRange(range);
    pointsInRange.insert(pointsInRange.end(), botRightInRange.begin(),
                         botRightInRange.end());

    return pointsInRange;
}

void QuadTree::subdivide() {
    Vector topLeftPosition =
        boundary.position.add(Vector(-boundary.width / 4, boundary.height / 4));
    Rectangle topLeftBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topLeftPosition);
    topLeft = new QuadTree(topLeftBoundary);

    Vector topRightPosition =
        boundary.position.add(Vector(boundary.width / 4, boundary.height / 4));
    Rectangle topRightBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topLeftPosition);
    topRight = new QuadTree(topRightBoundary);

    Vector botLeftPosition = boundary.position.add(
        Vector(-boundary.width / 4, -boundary.height / 4));
    Rectangle botLeftBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topLeftPosition);
    botLeft = new QuadTree(botLeftBoundary);

    Vector botRightPosition =
        boundary.position.add(Vector(boundary.width / 4, -boundary.height / 4));
    Rectangle botRightBoundary =
        Rectangle(boundary.width / 2, boundary.height / 2, topLeftPosition);
    botRight = new QuadTree(botRightBoundary);
}
