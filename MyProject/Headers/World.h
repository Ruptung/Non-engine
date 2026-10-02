#pragma once
#include "Storage.h"

class World {
public:
    World()
    : entityCounter(0){};

    Entity CreateEntity() {
        return entityCounter++;
    }



private:
    Entity entityCounter;
};
