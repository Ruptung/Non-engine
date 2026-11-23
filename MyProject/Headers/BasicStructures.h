#pragma once

struct  Vector2Int;

struct Vector2 {
public:
    // Data struct
    float x;
    float y;

    // Constructor
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(const Vector2Int &vec);
    static Vector2 zero() {
        return  {0, 0};
    }
    static Vector2 one() {
        return  {1, 1};
    }
    Vector2 normalize() const {
        float l = std::sqrt(x * x + y * y);

        if (l == 0) return zero();
        return{ x / l, y / l};
    }
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Basic operation
    // Addition logic
    Vector2 operator+(const Vector2 &other) const {
        return {x + other.x, y + other.y};
    }

    // Subtraction logic
    Vector2 operator-(const Vector2 &other) const {
        return {x - other.x, y - other.y};
    }

    // Multiplication logic
    Vector2 operator*(const float scalar) const {
        return {x * scalar, y * scalar};
    }
    Vector2 Scale(const Vector2 &other) const {
        return {x * other.x, y * other.y};
    }

    // Division logic
    Vector2 operator/(const float scalar) const {
        return {x / scalar, y / scalar};
    }
    Vector2 operator/(const Vector2 &other) const {
        return {x / other.x, y / other.y};
    }

    // logic
    bool operator==(const Vector2 other) const {
        return x == other.x && y == other.y;
    }
};

struct Vector2Int {
public:
    // Data struct
    int x;
    int y;

    // Constructor
    Vector2Int(int x, int y) : x(x), y(y) {}
    Vector2Int(const Vector2 &vec);
    static Vector2Int zero() {
        return  {0, 0};
    }
    static Vector2Int one() {
        return  {1, 1};
    }

    // Basic operation
    // Addition logic
    Vector2Int operator+(const Vector2Int &other) const {
        return {x + other.x, y + other.y};
    }

    // Subtraction logic
    Vector2Int operator-(const Vector2Int &other) const {
        return {x - other.x, y - other.y};
    }

    // Multiplication logic
    Vector2Int operator*(const int scalar) const {
        return {x * scalar, y * scalar};
    }
    Vector2Int Scale(const Vector2Int &other) const {
        return {x * other.x, y * other.y};
    }

    // Division logic
    Vector2Int operator/(const int scalar) const {
        return {x / scalar, y / scalar};
    }
    Vector2Int operator/(const Vector2Int &other) const {
        return {x / other.x, y / other.y};
    }

    // logic
    bool operator==(const Vector2Int other) const {
        return x == other.x && y == other.y;
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


// casting
inline Vector2::Vector2(const Vector2Int &vec) : x(vec.x), y(vec.y) {}

inline Vector2Int::Vector2Int(const Vector2 &vec) : x(std::floor(vec.x)), y(std::floor(vec.y)) {}