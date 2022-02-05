#pragma once
#include <SDL.h>
#include "Vector.h"
#include "Rectangle.h"
class RigidBody
{
	SDL_Surface* surface = nullptr;
public:
	Vector velocity;
	Vector acceleration;
	double accelerationRate;
	double mass;
	Rectangle hitbox;

	RigidBody(Vector startingPosition, SDL_Surface* surface,int width, int height, double accelerationRate);

	void move(double gameDelta);

	void draw(SDL_Surface* screen, Vector offset);

	void collide(RigidBody another, double gameDelta);

private:
	Rectangle leftHitbox();
	Rectangle rightHitbox();
	Rectangle topHitbox();
	Rectangle bottomHitbox();

};