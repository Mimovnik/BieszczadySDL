#include "Alive.h"
#include "RigidBody.h"
#include "Vector.h"

Alive::Alive(Vector startingPosition, SDL_Surface* surface,int width, int height, double accelerationRate) {
	this->hitbox.position = startingPosition;
	this->surface = surface;
	this->hitbox.width = width;
	this->hitbox.height = height;
	this->accelerationRate = accelerationRate;
}

void Alive::jump(RigidBody another){
    Vector below = Vector(0, 5);
	if (bottomHitbox().translate(below).overlaps(another.hitbox)) {
		acceleration.y = -20;
	}
}