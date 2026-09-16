#pragma once

struct Transform {
    Vector2 position;
};
struct Rhysic {
    Vector2 Velocity;
    Vector2 Acceleration;
};

class IRule {
public:
    virtual ~IRule() = default;
    virtual void Update() = 0;
};