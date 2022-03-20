#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#include <stdio.h>

#include <utility>

#define PI 3.14159265
class Vector {
   public:
    double x;
    double y;
    const static Vector ZERO;

    Vector() {
        x = 0;
        y = 0;
    }

    Vector(const Vector& other) {
        this->x = other.x;
        this->y = other.y;
    }

    Vector(Vector&& other) {
        this->x = std::move(other.x);
        this->y = std::move(other.y);
    }

    Vector(double x, double y) {
        this->x = x;
        this->y = y;
    }

    Vector& operator=(const Vector& right) {
        Vector tmp = right;
        std::swap(x, tmp.x);
        std::swap(y, tmp.y);
        return *this;
    }

    Vector& operator=(Vector&& right) {
        std::swap(x, right.x);
        std::swap(y, right.y);
        return *this;
    }

    Vector operator+(const Vector& right) { return this->add(right); }

    Vector& operator+=(const Vector& right) { return *this = this->add(right); }

    Vector operator-(const Vector& right) { return this->difference(right); }

    Vector& operator-=(const Vector& right) {
        return *this = this->difference(right);
    }

    Vector operator*(const double& right) { return this->rescale(right); }

    Vector operator*(const Vector& right) { return this->rescale(right); }

    Vector& operator*=(const double& right) {
        return *this = this->rescale(right);
    }

    Vector& operator*=(const Vector& right) {
        return *this = this->rescale(right);
    }

    static Vector fromAngle(double length, double angle) {
        double x = length * sin(angle * PI / 180);
        double y = length * cos(angle * PI / 180);
        return Vector(x, y);
    }

    Vector difference(Vector other) { return Vector(x - other.x, y - other.y); }

    Vector invert() { return Vector(-x, -y); }

    Vector invertX() { return Vector(-x, y); }

    Vector invertY() { return Vector(x, -y); }

    double magnitude() { return sqrt(x * x + y * y); }

    Vector rescale(double factor) { return Vector(x * factor, y * factor); }

    Vector rescale(Vector other) {
        double ratio = other.magnitude() / this->magnitude();
        return rescale(ratio);
    }

    Vector add(Vector other) { return Vector(x + other.x, y + other.y); }

    Vector setAngle(double angle) {
        return Vector(magnitude() * sin(angle * PI / 180),
                      magnitude() * cos(angle * PI / 180));
    }

    double getAngle() {
        if (magnitude() == 0) return 0;
        return (180 / PI) * atan2(x, y);
    }

    Vector rotate(double angle) { return setAngle(getAngle() + angle); }
};
#endif
