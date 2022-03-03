#include "RigidBody.h"
#include "settings.h"
#include <iostream>

void RigidBody::move(double gameDelta, RigidBody* others, int othersCount) {
    Vector friction = Vector::ZERO;
    Vector below = Vector(0, 5);
    Rectangle* othersHitboxes = new Rectangle[othersCount];
    for (int i = 0; i < othersCount; i++) {
        othersHitboxes[i] = others[i].hitbox;
    }
    if (bottomHitbox().translate(below).overlapsAny(othersHitboxes, othersCount)) {
        if (velocity.x > 0) {
            friction = Vector(FRICTION_FACTOR, 0);
        } else if (velocity.x < 0) {
            friction = Vector(-FRICTION_FACTOR, 0);
        }
    }
    delete[] othersHitboxes;

    acceleration = acceleration.difference(friction);
    if((velocity.x >= 0 && velocity.add(acceleration.rescale(gameDelta)).x >= 0) || (velocity.x <= 0 && velocity.add(acceleration.rescale(gameDelta)).x <= 0)){
        velocity = velocity.add(acceleration.rescale(gameDelta));
    } else{
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
    hitbox.position = hitbox.position.add(velocity.rescale(gameDelta));
}

void RigidBody::draw(SDL_Surface* screen, Vector offset) {
    SDL_Rect dest;
    if(active != nullptr){
    if (drawScaledToHitbox) {
        dest.x = static_cast<int>(hitbox.position.x - hitbox.width / 2 - offset.x);
        dest.y = static_cast<int>(hitbox.position.y - hitbox.height / 2 - offset.y);
        dest.w = static_cast<int>(hitbox.width);
        dest.h = static_cast<int>(hitbox.height);
        SDL_BlitScaled(active->currentSurface, NULL, screen, &dest);
    } else {
        dest.x = static_cast<int>(hitbox.position.x - active->currentSurface->w / 2 - offset.x);
        dest.y = static_cast<int>(hitbox.position.y - active->currentSurface->h / 2 - offset.y);
        dest.w = active->currentSurface->w;
        dest.h = active->currentSurface->h;
        SDL_BlitScaled(active->currentSurface, NULL, screen, &dest);
    }
    }
    else{
        if (drawScaledToHitbox) {
        dest.x = static_cast<int>(hitbox.position.x - hitbox.width / 2 - offset.x);
        dest.y = static_cast<int>(hitbox.position.y - hitbox.height / 2 - offset.y);
        dest.w = static_cast<int>(hitbox.width);
        dest.h = static_cast<int>(hitbox.height);
        SDL_BlitScaled(surface, NULL, screen, &dest);
    } else {
        dest.x = static_cast<int>(hitbox.position.x - surface->w / 2 - offset.x);
        dest.y = static_cast<int>(hitbox.position.y - surface->h / 2 - offset.y);
        dest.w = surface->w;
        dest.h = surface->h;
        SDL_BlitScaled(surface, NULL, screen, &dest);
    }
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
        futureOffset = velocity.rescale(gameDelta).add(
            acceleration.rescale(gameDelta * gameDelta / 2));
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
