#pragma once
#include "BasicStructures.h"

struct Transform {
    Transform(Vector2 position) : position(position) {
    }

    Vector2 position;
};
struct Physic {
    Physic() : Velocity(Vector2::zero()), Acceleration(Vector2::zero()) {}

    Vector2 Velocity;
    Vector2 Acceleration;
};

struct Renderable {
    Renderable(Transform *transform, uint16_t tileData) : transform(transform), tileData(tileData) {}

    Transform *transform;
    uint16_t tileData;
};

class IRule {
public:
    virtual ~IRule() = default;
    virtual void Update() = 0;
};