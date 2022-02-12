#include<math.h>
#include<stdio.h>

#ifndef VECTOR_H
#define VECTOR_H

#define PI 3.14159265
class Vector
{
public:
	double x;
	double y;
	const static Vector ZERO;

	Vector() {
		x = 0;
		y = 0;
	}

	Vector(double x, double y) {
		this->x = x;
		this->y = y;
	}

	static Vector fromAngle(double length, double angle) {
		double x = length * sin(angle * PI / 180);
		double y = length * cos(angle * PI / 180);
		return Vector(x, y);
	}

	Vector difference(Vector other) {
		return Vector(x - other.x, y - other.y);
	}

	Vector invert() {
		return Vector(-x, -y);
	}

	Vector invertX() {
		return Vector(-x, y);
	}

	Vector invertY() {
		return Vector(x, -y);
	}

	double magnitude() {
		return sqrt(x * x + y * y);
	}

	Vector rescale(double factor) {
		return Vector(x * factor, y * factor);
	}

	Vector rescale(Vector other) {
		double ratio = other.magnitude() / this->magnitude();
		return rescale(ratio);
	}

	Vector add(Vector other) {
		return Vector(x + other.x, y + other.y);
	}

	Vector setAngle(double angle) {
		return Vector(magnitude() * sin(angle * PI / 180), magnitude() * cos(angle * PI / 180));
	}

	double getAngle() {
		if (magnitude() == 0) return 0;
		return (180 / PI) * atan2(x, y);
	}

	Vector rotate(double angle) {
		return setAngle(getAngle() + angle);
	}
};
#endif
