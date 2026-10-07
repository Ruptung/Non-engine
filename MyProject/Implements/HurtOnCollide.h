#pragma once

#include <cstdio>

#include "Headers/ICollide.h"

class HurtOnCollide : public ICollide {
public:
    void Collide(Entity other) override {
        printf("Hurt %d\n", other);
    }
private:
};
