#pragma once

#include <cstdint>

#include "../BasicStructures.h"

class IScript {
public:
    virtual ~IScript() = default;

    virtual void Start() = 0;

    virtual void Update() = 0;
private:
    Entity thisEntity;
};
