#include "RigidBody.h"

RigidBody::RigidBody(Vector startingPosition, SDL_Surface* surface, double accelerationRate) {
	this->position = startingPosition;
	this->surface = surface;
	this->accelerationRate = accelerationRate;
}


void RigidBody::move(double delta) {
	const double frictionFactor = 0.1;
	velocity = velocity.difference(velocity.rescale(frictionFactor));
	position = position.add(velocity.rescale(delta));
}

void RigidBody::draw(SDL_Surface* screen, Vector offset) {
	SDL_Rect dest;
	dest.x = position.x - surface->w / 2;
	dest.y = position.y - surface->h / 2;
	dest.w = surface->w;
	dest.h = surface->h;
	SDL_BlitSurface(surface, NULL, screen, &dest);
}