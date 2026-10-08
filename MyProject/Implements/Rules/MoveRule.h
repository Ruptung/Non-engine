#pragma once

#include <cmath>

#include "Headers/BasicStructures.h"
#include "../../Headers/Interfaces/IRule.h"
#include "Headers/Storage.h"
#include "Headers/Object.h"

#include "Implements/EventListeners/BasicKeyTest.h"

enum WorldFlag : uint16_t {
    FLAG_NONE       = 0,
    FLAG_SOLID      = 1 << 0,
    // WIP ...
};

class MoveRule : public IRule {
public:
    MoveRule(Storage<Transform> &transforms, Entity target, KeyFlags &keys, Vector2 &WorldSize, uint16_t *WorldSpec)
    : transforms(transforms),
    target(target),
    WorldSize(WorldSize),
    WorldSpec(WorldSpec),
    keys(keys) {
    }

    void Update() override {
        Transform &player = transforms.Get(target);

        if (!CanMove(player.position, keys.axis, player.scale))
            return;

        player.position = player.position + keys.axis;
    }
    
private:
    bool CanMove(Vector2 pos, Vector2 moveto, Vector2 scale) {
        if (scale.x * scale.y == 0) { return true; }
        else { scale = scale - Vector2::one(); }

        Vector2 pivot1 = pos + moveto;
        Vector2 pivot2 = pivot1 + scale;

        if (moveto.x > 0) {
            pivot1.x += scale.x;
        }else if (moveto.x < 0) {
            pivot2.x -= scale.x;
        }

        if (moveto.y > 0) {
            pivot1.y += scale.y;
        }else if (moveto.y < 0) {
            pivot2.y -= scale.y;
        }

        //TODO Make this divide by 4 to Grid/Tile Size.
        int flatten1 = std::ceil(pivot1.y / 4) * WorldSize.x / 4 + std::ceil(pivot1.x / 4);
        int flatten2 = std::ceil(pivot2.y / 4) * WorldSize.x / 4 + std::ceil(pivot2.x / 4);

        if ((pivot1.x >= 0 && pivot1.x < WorldSize.x) && (pivot1.y >= 0 && pivot1.y < WorldSize.y)) {
            if (WorldSpec[flatten1] & FLAG_SOLID) {
                return false;
            }
        }
        if ((pivot2.x >= 0 && pivot2.x < WorldSize.x) && (pivot2.y >= 0 && pivot2.y < WorldSize.y)) {
            if (WorldSpec[flatten2] & FLAG_SOLID) {
                return false;
            }
        }
        return true;
    }

    Storage<Transform> &transforms;
    Entity target;
    Vector2 &WorldSize;
    uint16_t *WorldSpec;
    KeyFlags &keys;
};
