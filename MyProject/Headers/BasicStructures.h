#pragma once

#include <cmath>
struct Vector2 {
public:
    Vector2(int x, int y) : x(x), y(y) {}

    int x;
    int y;

    Vector2 operator+(const Vector2 other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2 other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(const int scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(const int scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2 operator/(const Vector2 other) const {
        return Vector2(std::floor((float)x / other.x), std::floor((float)y / other.y));
    }

    bool operator==(const Vector2 other) const {
        return x == other.x && y == other.y;
    }

    Vector2 Scale(const Vector2 other) const {
        return Vector2(x * other.x, y * other.y);
    }

    static Vector2 zero() {
        return  Vector2(0, 0);
    }
    static Vector2 one() {
        return  Vector2(1, 1);
    }
};

struct ScreenSet {
    ScreenSet(Vector2 screen_vector, Vector2 virtual_vector)
    : ScreenVector(screen_vector),
    VirtualVector(virtual_vector) {
    }

    Vector2 ScreenVector;
    Vector2 VirtualVector;
};