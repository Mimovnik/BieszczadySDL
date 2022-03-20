#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Vector.h"
#include <utility>

class Rectangle {
   public:
    Vector position;
    double width;
    double height;

    Rectangle() {}

    Rectangle(const Rectangle& other) {
        this->position = other.position;
        this->width = other.width;
        this->height = other.height;
    }

    Rectangle(Rectangle&& other) {
        this->position = std::move(other.position);
        this->width = std::move(other.width);
        this->height = std::move(other.height);
    }

    Rectangle(double width, double height) {
        this->width = width;
        this->height = height;
    }

    Rectangle(double width, double height, Vector position) {
        this->width = width;
        this->height = height;
        this->position = position;
    }

    Rectangle& operator=(const Rectangle& right) {
        Rectangle tmp = right;
        std::swap(position, tmp.position);
        std::swap(width, tmp.width);
        std::swap(height, tmp.height);
        return *this;
    }

    Rectangle& operator=(Rectangle&& right) {
        std::swap(position, right.position);
        std::swap(width, right.width);
        std::swap(height, right.height);
        return *this;
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