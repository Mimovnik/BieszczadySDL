#include "Rectangle.h"

#include "Vector.h"

bool Rectangle::contains(Vector point) {
    double thisLeftSide = position.x - width / 2;
    double thisRightSide = position.x + width / 2;
    double thisUpperSide = position.y - height / 2;
    double thisBottomSide = position.y + height / 2;
    if (point.x >= thisLeftSide && point.x <= thisRightSide &&
        point.y >= thisUpperSide && point.y <= thisBottomSide) {
        return true;
    }
    return false;
}

Rectangle Rectangle::translate(Vector offset) {
    Rectangle translated = *this;
    translated.position.x += offset.x;
    translated.position.y += offset.y;
    return translated;
}

bool Rectangle::overlaps(Rectangle another) {
    double thisLeftSide = position.x - width / 2;
    double thisRightSide = position.x + width / 2;
    double thisTopSide = position.y - height / 2;
    double thisBottomSide = position.y + height / 2;

    double anotherLeftSide = another.position.x - another.width / 2;
    double anotherRightSide = another.position.x + another.width / 2;
    double anotherTopSide = another.position.y - another.height / 2;
    double anotherBottomSide = another.position.y + another.height / 2;

    return thisBottomSide > anotherTopSide && thisTopSide < anotherBottomSide &&
           thisLeftSide < anotherRightSide && thisRightSide > anotherLeftSide;
}

bool Rectangle::overlapsAny(Rectangle* others, int othersCount) {
    double thisLeftSide = position.x - width / 2;
    double thisRightSide = position.x + width / 2;
    double thisTopSide = position.y - height / 2;
    double thisBottomSide = position.y + height / 2;

    double anotherLeftSide;
    double anotherRightSide;
    double anotherTopSide;
    double anotherBottomSide;

    for (int i = 0; i < othersCount; i++) {
        anotherLeftSide = others[i].position.x - others[i].width / 2;
        anotherRightSide = others[i].position.x + others[i].width / 2;
        anotherTopSide = others[i].position.y - others[i].height / 2;
        anotherBottomSide = others[i].position.y + others[i].height / 2;
        if (thisBottomSide > anotherTopSide &&
            thisTopSide < anotherBottomSide &&
            thisLeftSide < anotherRightSide &&
            thisRightSide > anotherLeftSide) {
            return true;
        }
    }
    return false;
}
