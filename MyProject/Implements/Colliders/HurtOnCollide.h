#pragma once

#include <cstdio>

#include "../../Headers/Interfaces/ICollide.h"

class HurtOnCollide : public ICollide {
private:
    Storage<Zombie> *zombies;
public:
    HurtOnCollide(Storage<Zombie> *zombies)
    : zombies(zombies)
    {

    }
    void Collide(Entity other) override {
        if (zombies->Has(other)) {
            printf("Game Over\n");
        }
    }
};
