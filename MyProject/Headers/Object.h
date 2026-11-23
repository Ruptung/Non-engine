#pragma once

#include "BasicStructures.h"

struct Transform {
    Transform(Vector2 position)
    : position(position),
    extend(Vector2::zero()) {
    }

    Transform(Vector2 position, Vector2 extend)
    : position(position),
    extend(extend) {
    }

    Vector2 position;
    Vector2 extend;
};

struct Physics {
    Physics() : Velocity(Vector2::zero()) {}

    Vector2 Velocity;
};

struct Renderable {
    Renderable(uint16_t tileData) : tileData(tileData) {}

    uint16_t tileData;
};

struct Collider {
    // isTrigger(it Stops?) 1, WIP...
    uint8_t collideData;
};

struct Zombie {
};

struct Creature {
    Creature(float Health, float HealthDelta)
    : Health(Health),
    HealthDelta(HealthDelta) {
    }

    float Health;
    float HealthDelta;
};
