#pragma once

#include "Storage.h"

class ICollide {
public:
    virtual ~ICollide() = default;

    virtual void Collide(Entity other) = 0;
};
