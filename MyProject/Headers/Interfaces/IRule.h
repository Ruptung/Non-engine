#pragma once

#include <cstdint>

#include "../BasicStructures.h"

class IRule {
public:
    virtual ~IRule() = default;
    virtual void Update() = 0;
};
