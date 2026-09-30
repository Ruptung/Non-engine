#pragma once

#include <vector>

#include "Headers/BasicStructures.h"
#include "Headers/IRule.h"

#include "Implements/EventListeners/BasicKeyTest.h"

enum WorldFlag : uint16_t {
    FLAG_NONE       = 0,
    FLAG_SOLID      = 1 << 0,
    // WIP ...
};

class MoveRule : public IRule {
public:
    MoveRule(Transform &player, KeyFlags &keys, Vector2 &WorldSize, uint16_t *WorldSpec) :
    player(player), keys(keys), WorldSize(WorldSize), WorldSpec(WorldSpec) {};
    void Update() override {

        //Move
        Vector2 moveto = player.position + keys.axis;

        if ((moveto.x >= 0 && moveto.x < WorldSize.x) && (moveto.y >= 0 && moveto.y < WorldSize.y)) {
            int flatten = moveto.y * WorldSize.x + moveto.x;

            if (WorldSpec[flatten] & WorldFlag::FLAG_SOLID)
                return;

        }
        player.position = player.position + keys.axis;
    }
private:
    Transform &player;
    Vector2 &WorldSize;
    uint16_t *WorldSpec;
    KeyFlags &keys;
};
