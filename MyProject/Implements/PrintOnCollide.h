#pragma once

#include <cstdio>

#include "../Headers/Interfaces/ICollide.h"

class PrintOnCollide : public ICollide {
public:
    void Collide(Entity other) override {
        printf("Collide %d\n", other);
    }
private:
};
