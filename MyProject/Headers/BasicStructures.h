#pragma once

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

    bool operator==(const Vector2 other) const {
        return x == other.x && y == other.y;
    }

    static Vector2 zero() {
        return  Vector2(0, 0);
    }
    static Vector2 one() {
        return  Vector2(1, 1);
    }
};
