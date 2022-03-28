#include "RigidBody.h"

#include <iostream>

#include "../functions/settings.h"

void RigidBody::move(double gameDelta) {
    Vector friction = Vector::ZERO;
    Vector below = Vector(0, 5);
    Rectangle* othersHitboxes = new Rectangle[collidersCount];
    for (int i = 0; i < collidersCount; i++) {
        othersHitboxes[i] = colliders[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlapsAny(othersHitboxes,
                                                    collidersCount)) {
        if (velocity.x > 0) {
            friction = Vector(FRICTION_FACTOR, 0);
        } else if (velocity.x < 0) {
            friction = Vector(-FRICTION_FACTOR, 0);
        }
    }
    delete[] othersHitboxes;

    acceleration -= friction;
    if ((velocity.x >= 0 && (velocity + (acceleration * gameDelta)).x >= 0) ||
        (velocity.x <= 0 && (velocity + (acceleration * gameDelta)).x <= 0)) {
        velocity += acceleration * gameDelta;
    } else {
        velocity.x = 0;
    }

    if (velocity.x < MIN_SPEED && velocity.x > -MIN_SPEED) {
        velocity.x = 0;
    }
    if (velocity.y < MIN_SPEED && velocity.y > -MIN_SPEED) {
        velocity.y = 0;
    }
    if (velocity.x > maxSpeed) velocity.x = maxSpeed;
    if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;
    hitbox.position += velocity * gameDelta;
}

void RigidBody::collide(double gameDelta) {
    Vector futureOffset =
        velocity * gameDelta + acceleration * (gameDelta * gameDelta / 2);

    std::vector<Rectangle> collidableHitboxList;
    for (int i = 0; i < collidersCount; i++) {
        if (colliders[i].collidable)
            collidableHitboxList.push_back(colliders[i].hitbox);
    }

    int collidableCount = static_cast<int>(collidableHitboxList.size());
    Rectangle* collidableHitboxes = new Rectangle[collidableCount];
    std::copy(collidableHitboxList.begin(), collidableHitboxList.end(),
              collidableHitboxes);

    if (topHitbox()
            .translate(futureOffset)
            .overlapsAny(collidableHitboxes, collidableCount) ||
        bottomHitbox()
            .translate(futureOffset)
            .overlapsAny(collidableHitboxes, collidableCount)) {
        velocity.y = 0;
        acceleration.y = 0;
        futureOffset =
            velocity * gameDelta + (acceleration * (gameDelta * gameDelta / 2));
    }
    if (leftHitbox()
            .translate(futureOffset)
            .overlapsAny(collidableHitboxes, collidableCount) ||
        rightHitbox()
            .translate(futureOffset)
            .overlapsAny(collidableHitboxes, collidableCount)) {
        velocity.x = 0;
        acceleration.x = 0;
    }
    delete[] collidableHitboxes;
}

Vector RigidBody::futureOffset(double gameDelta) {
    return velocity * gameDelta + (acceleration * (gameDelta * gameDelta / 2));
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
