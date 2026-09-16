#pragma once

#include "VirtualScreen.h"

struct Renderable {
    Renderable(Vector2 pivot, uint16_t tileData) : pivot(pivot), tileData(tileData) {
    }

    Vector2 pivot;
    uint16_t tileData;
};

class IRender {
public:
    virtual ~IRender() = default;
    virtual void Render(VirtualScreen &vs) = 0;
};
