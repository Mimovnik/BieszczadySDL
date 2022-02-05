#include "RigidBody.h"

RigidBody::RigidBody(Vector startingPosition, SDL_Surface* surface,int width, int height, double accelerationRate) {
	this->hitbox.position = startingPosition;
	this->surface = surface;
	this->accelerationRate = accelerationRate;
	this->hitbox.width = width;
	this->hitbox.height = height;
}


void RigidBody::move(double gameDelta) {
	// const double frictionFactor = 0.5;
	// Vector friction = velocity.rescale(frictionFactor);
	// Vector netAcceleration = acceleration.difference(friction);
	velocity = velocity.add(acceleration.rescale(gameDelta));
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

Collision RigidBody::collide(RigidBody another, double gameDelta) {
	Rectangle shadowHitbox = this->hitbox.translate(this->velocity.rescale(gameDelta));
	Collision collision;
	if (shadowHitbox.overlaps(another.hitbox)) {
		collision.exists = true;
	}
	else {
		collision.exists = false;
	}
	if (leftHitbox().translate(this->velocity.rescale(gameDelta)).overlaps(another.hitbox) || rightHitbox().translate(this->velocity.rescale(gameDelta)).overlaps(another.hitbox)) {
		collision.acceleration.x = 0;
	}
	if (topHitbox().translate(this->velocity.rescale(gameDelta)).overlaps(another.hitbox) || bottomHitbox().translate(this->velocity.rescale(gameDelta)).overlaps(another.hitbox)) {
		collision.acceleration.y = 0;
	}
	return collision;
}

Rectangle RigidBody::leftHitbox() {
	Rectangle leftHitbox;
	leftHitbox.width = 10;
	leftHitbox.height = hitbox.height;
	leftHitbox.position.x = hitbox.leftSide() + 5;
	leftHitbox.position.y = hitbox.position.y;
	return leftHitbox;
}

Rectangle RigidBody::rightHitbox() {
	Rectangle rightHitbox;
	rightHitbox.width = 10;
	rightHitbox.height = hitbox.height;
	rightHitbox.position.x = hitbox.rightSide() - 5;
	rightHitbox.position.y = hitbox.position.y;
	return rightHitbox;
}

Rectangle RigidBody::topHitbox() {
	Rectangle topHitbox;
	topHitbox.width = hitbox.width;
	topHitbox.height = 10;
	topHitbox.position.x = hitbox.position.x;
	topHitbox.position.y = hitbox.topSide() + 5;
	return topHitbox;
}

Rectangle RigidBody::bottomHitbox() {
	Rectangle botHitbox;
	botHitbox.width = hitbox.width;
	botHitbox.height = 10;
	botHitbox.position.x = hitbox.position.x;
	botHitbox.position.y = hitbox.bottomSide() - 5;
	return botHitbox;
}

