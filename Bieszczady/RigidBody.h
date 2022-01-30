#pragma once
#include <SDL.h>
#include "Vector.h"
class RigidBody
{
	SDL_Surface* surface = nullptr;
public:
	Vector position;
	Vector velocity;
	double accelerationRate;


	RigidBody(Vector startingPosition, SDL_Surface* surface, double accelerationRate);

	void move(double gameTime);

	void draw(SDL_Surface* screen, Vector offset);

};

