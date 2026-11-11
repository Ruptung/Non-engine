#pragma once

#include <cmath>
struct Vector2Int {
public:
    Vector2Int(int x, int y) : x(x), y(y) {}

    int x;
    int y;

    Vector2Int operator+(const Vector2Int other) const {
        return Vector2Int(x + other.x, y + other.y);
    }

    Vector2Int operator-(const Vector2Int other) const {
        return Vector2Int(x - other.x, y - other.y);
    }

    Vector2Int operator*(const int scalar) const {
        return Vector2Int(x * scalar, y * scalar);
    }

    Vector2Int operator/(const int scalar) const {
        return Vector2Int(x / scalar, y / scalar);
    }

    Vector2Int operator/(const Vector2Int other) const {
        return Vector2Int(std::floor((float)x / other.x), std::floor((float)y / other.y));
    }

    Vector2Int *operator+=(const Vector2Int &other) {
        x += other.x;
        y += other.y;
        return this;
    }

    bool operator==(const Vector2Int other) const {
        return x == other.x && y == other.y;
    }

    Vector2Int Scale(const Vector2Int other) const {
        return Vector2Int(x * other.x, y * other.y);
    }

    static Vector2Int zero() {
        return  Vector2Int(0, 0);
    }
    static Vector2Int one() {
        return  Vector2Int(1, 1);
    }
};

struct ScreenSet {
    ScreenSet(Vector2Int screen_vector, Vector2Int virtual_vector)
    : ScreenVector(screen_vector),
    VirtualVector(virtual_vector) {
    }

    Vector2Int ScreenVector;
    Vector2Int VirtualVector;
};