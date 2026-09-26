#pragma once

#include <cstdint>

#include "IRule.h"
#include "VirtualScreen.h"

class IRender {
public:
    virtual ~IRender() = default;
    virtual void Render(VirtualScreen &vs) = 0;
};
