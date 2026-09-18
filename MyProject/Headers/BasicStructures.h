#pragma once

struct Vector2 {
public:
    Vector2(int x, int y) : x(x), y(y) {}

    int x;
    int y;

    Vector2 operator+(const Vector2 other) {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator*(const int scalar) {
        return Vector2(x * scalar, y * scalar);
    }

    static Vector2 zero() {
        return  Vector2(0, 0);
    }
};