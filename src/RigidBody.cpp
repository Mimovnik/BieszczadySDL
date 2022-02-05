#include "RigidBody.h"

RigidBody::RigidBody(Vector startingPosition, SDL_Surface* surface, int width,
                     int height, double accelerationRate) {
    this->hitbox.position = startingPosition;
    this->surface = surface;
    this->hitbox.width = width;
    this->hitbox.height = height;
    this->accelerationRate = accelerationRate;
}

void RigidBody::calculatePosition(double gameDelta, RigidBody another) {
    const double frictionFactor = 2;
    Vector friction = Vector::ZERO;
    Vector below = Vector(0, 5);
    if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
        if (velocity.x > 0) {
            friction = Vector(frictionFactor, 0);
        } else if (velocity.x < 0) {
            friction = Vector(-frictionFactor, 0);
        }
    }
    Vector netAcceleration = acceleration.difference(friction);
    velocity = velocity.add(netAcceleration.rescale(gameDelta));
    hitbox.position = hitbox.position.add(velocity.rescale(gameDelta));
}

void RigidBody::draw(SDL_Surface* screen, Vector offset) {
    SDL_Rect dest;
    dest.x = hitbox.position.x - hitbox.width / 2 - offset.x;
    dest.y = hitbox.position.y - hitbox.height / 2 - offset.y;
    dest.w = hitbox.width;
    dest.h = hitbox.height;
    SDL_BlitScaled(surface, NULL, screen, &dest);
}

void RigidBody::collide(RigidBody another, double gameDelta) {
    Vector futureOffset = velocity.rescale(gameDelta).add(
        acceleration.rescale(gameDelta * gameDelta / 2));
    if (topHitbox().translate(futureOffset).overlaps(another.hitbox) ||
        bottomHitbox().translate(futureOffset).overlaps(another.hitbox)) {
        velocity.y = 0;
        acceleration.y = 0;
        futureOffset = velocity.rescale(gameDelta).add(
            acceleration.rescale(gameDelta * gameDelta / 2));
    }
    if (leftHitbox().translate(futureOffset).overlaps(another.hitbox) ||
        rightHitbox().translate(futureOffset).overlaps(another.hitbox)) {
        velocity.x = 0;
        acceleration.x = 0;
    }
}

Rectangle RigidBody::leftHitbox() {
    Rectangle leftHitbox;
    leftHitbox.width = 10;
    leftHitbox.height = hitbox.height - 2;
    leftHitbox.position.x = hitbox.leftSide() + 5;
    leftHitbox.position.y = hitbox.position.y;
    return leftHitbox;
}

Rectangle RigidBody::rightHitbox() {
    Rectangle rightHitbox;
    rightHitbox.width = 10;
    rightHitbox.height = hitbox.height - 2;
    rightHitbox.position.x = hitbox.rightSide() - 5;
    rightHitbox.position.y = hitbox.position.y;
    return rightHitbox;
}

Rectangle RigidBody::topHitbox() {
    Rectangle topHitbox;
    topHitbox.width = hitbox.width - 2;
    topHitbox.height = 10;
    topHitbox.position.x = hitbox.position.x;
    topHitbox.position.y = hitbox.topSide() + 5;
    return topHitbox;
}

Rectangle RigidBody::bottomHitbox() {
    Rectangle botHitbox;
    botHitbox.width = hitbox.width - 2;
    botHitbox.height = 10;
    botHitbox.position.x = hitbox.position.x;
    botHitbox.position.y = hitbox.bottomSide() - 5;
    return botHitbox;
}
