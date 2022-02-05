#pragma once
#include "Vector.h"

class Rectangle
{
public:
	Vector position;
	int width;
	int height;

	Rectangle() {
	}

	Rectangle( int width, int height) {
		this->width = width;
		this->height = height;
	}

	bool contains(Vector point);

	bool overlaps(Rectangle another);

	Rectangle translate(Vector offset);

	double rightSide() {
		return position.x + width / 2;
	}

	double leftSide() {
		return position.x - width / 2;
	}

	double topSide() {
		return position.y - height / 2;
	}

	double bottomSide() {
		return position.y + height / 2;
	}

	
};

