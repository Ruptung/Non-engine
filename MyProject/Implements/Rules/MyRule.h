#pragma once

#include <vector>

#include "../../Headers/BasicStructures.h"

#include "../../Headers/IRule.h"
#include "../EventListeners/BasicKeyTest.h"

enum WorldFlag : uint16_t {
    FLAG_NONE       = 0,
    FLAG_SOLID      = 1 << 0,
    // WIP ...
};

class MyRule : public IRule {
public:
    MyRule(KeyFlags &keys, uint16_t *WorldSpec, std::vector<Transform*>& transforms) :
    transforms(transforms), WorldSpec(WorldSpec), keys(keys) {};
    void Update() override {
        Vector2 moveto = transforms[1]->position + keys.axis;


        if ((moveto.x >= 0 && moveto.x < 5) && (moveto.y >= 0 && moveto.y < 5)) {

            int flatten = moveto.y * 5 + moveto.x;

            if (WorldSpec[flatten] & WorldFlag::FLAG_SOLID)
                return;

        }


        transforms[1]->position = transforms[1]->position + keys.axis;
    }
private:
    std::vector<Transform*> &transforms;
    uint16_t *WorldSpec;
    KeyFlags &keys;
};