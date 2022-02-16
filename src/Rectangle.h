#include "Vector.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H
class Rectangle {
   public:
    Vector position;
    double width;
    double height;

    Rectangle() {}

    Rectangle(double width, double height) {
        this->width = width;
        this->height = height;
    }

    Rectangle(double width, double height, Vector position) {
        this->width = width;
        this->height = height;
        this->position = position;
    }

    bool contains(Vector point);

    bool overlaps(Rectangle another);
    bool overlapsAny(Rectangle* others, int othersCount);

    Rectangle translate(Vector offset);

    double rightSide() { return position.x + width / 2; }

    double leftSide() { return position.x - width / 2; }

    double topSide() { return position.y - height / 2; }

    double bottomSide() { return position.y + height / 2; }
};

#endif