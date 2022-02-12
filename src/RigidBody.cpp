#include "RigidBody.h"

#include <iostream>

void RigidBody::move(double gameDelta, RigidBody* others, int othersCount) {
    const double frictionFactor = 7;
    Vector friction = Vector::ZERO;
    Vector below = Vector(0, 5);
    Rectangle* othersHitboxes = new Rectangle[othersCount];
    for (int i = 0; i < othersCount; i++) {
        othersHitboxes[i] = others[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlaps(othersHitboxes, othersCount)) {
        if (velocity.x > 0) {
            friction = Vector(frictionFactor, 0);
        } else if (velocity.x < 0) {
            friction = Vector(-frictionFactor, 0);
        }
    }
    delete[] othersHitboxes;

    acceleration = acceleration.difference(friction);
    if ((velocity.add(acceleration.rescale(gameDelta)).magnitude() > 0 &&
            velocity.magnitude() > 0 )||
        (velocity.add(acceleration.rescale(gameDelta)).magnitude() < 0 &&
            velocity.magnitude() < 0)) {

        velocity = velocity.add(acceleration.rescale(gameDelta));
    } else {
        velocity = Vector::ZERO;
    }

    if (velocity.x < 0.1 && velocity.x > -0.1) {
        velocity.x = 0;
    }
    if (velocity.y < 0.1 && velocity.y > -0.1) {
        velocity.y = 0;
    }
    if (velocity.x > maxSpeed) velocity.x = maxSpeed;
    if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;
    // if (velocity.y < -maxSpeed) velocity.y = -maxSpeed;
    // if (velocity.y > maxSpeed) velocity.y = maxSpeed;
    hitbox.position = hitbox.position.add(velocity.rescale(gameDelta));
}

void RigidBody::draw(SDL_Surface* screen, Vector offset) {
    SDL_Rect dest;
    if (drawScaledToHitbox) {
        dest.x = hitbox.position.x - hitbox.width / 2 - offset.x;
        dest.y = hitbox.position.y - hitbox.height / 2 - offset.y;
        dest.w = hitbox.width;
        dest.h = hitbox.height;
        SDL_BlitScaled(active.currentSurface, NULL, screen, &dest);
    } else {
        dest.x = hitbox.position.x - active.currentSurface->w / 2 - offset.x;
        dest.y = hitbox.position.y - active.currentSurface->h / 2 - offset.y;
        dest.w = active.currentSurface->w;
        dest.h = active.currentSurface->h;
        SDL_BlitScaled(active.currentSurface, NULL, screen, &dest);
    }
}

void RigidBody::collide(RigidBody* others, int othersCount, double gameDelta) {
    Vector futureOffset = velocity.rescale(gameDelta).add(
        acceleration.rescale(gameDelta * gameDelta / 2));

    std::vector<Rectangle> collidableHitboxList;
    for (int i = 0; i < othersCount; i++) {
        if (others[i].collidable)
            collidableHitboxList.push_back(others[i].hitbox);
    }

    int collidableCount = collidableHitboxList.size();
    Rectangle* collidableHitboxes = new Rectangle[collidableCount];
    std::copy(collidableHitboxList.begin(), collidableHitboxList.end(),
              collidableHitboxes);

    if (topHitbox()
            .translate(futureOffset)
            .overlaps(collidableHitboxes, collidableCount) ||
        bottomHitbox()
            .translate(futureOffset)
            .overlaps(collidableHitboxes, collidableCount)) {
        velocity.y = 0;
        acceleration.y = 0;
        futureOffset = velocity.rescale(gameDelta).add(
            acceleration.rescale(gameDelta * gameDelta / 2));
    }
    if (leftHitbox()
            .translate(futureOffset)
            .overlaps(collidableHitboxes, collidableCount) ||
        rightHitbox()
            .translate(futureOffset)
            .overlaps(collidableHitboxes, collidableCount)) {
        velocity.x = 0;
        acceleration.x = 0;
    }
    delete[] collidableHitboxes;
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
