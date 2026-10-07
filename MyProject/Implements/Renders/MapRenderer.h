#pragma once

#include "Headers/BasicStructures.h"
#include "Headers/IRender.h"

class MapRenderer : public IRender {
public:
    MapRenderer(uint16_t *map, Vector2 worldSize, Vector2 screenSize, Storage<Transform> &transforms, const Entity target)
    : map(map), worldSize(worldSize), screenSize(screenSize), transforms(transforms), target(target){
    }

    void Render(VirtualScreen &vs) override {
        Vector2 camera = transforms.Get(target).position;

        for (int y = 0; y < screenSize.y; y++) {
            for (int x = 0; x < screenSize.x; x++) {
                Vector2 pos = camera + Vector2(x, y) - screenSize / 2;

                if (pos.x < 0 || pos.x >= worldSize.x) continue;
                if (pos.y < 0 || pos.y >= worldSize.y) continue;

                uint16_t tileIndex = map[pos.y * worldSize.x + pos.x];

                vs.DrawTileOnWorld(pos * 4, tileIndex);
            }
        }
    }

private:
    uint16_t *map;
    Vector2 worldSize;
    Vector2 screenSize;
    Storage<Transform> &transforms;
    const Entity target;
};