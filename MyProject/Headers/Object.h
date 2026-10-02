#pragma once

#include <cstdint>
#include <vector>

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
    Renderable(uint16_t tileData) : tileData(tileData) {}

    uint16_t tileData;
};
