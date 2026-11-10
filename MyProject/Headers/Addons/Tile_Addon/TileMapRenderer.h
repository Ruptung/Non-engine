#pragma once

#include "Headers/BasicStructures.h"
#include "TilePainter.h"
#include "Headers/Interfaces/IRender.h"

class TileMapRenderer : public IRender {
public:
    TileMapRenderer(TilePainter &TilePainter, const ScreenSet &ScreenSet, const TileSet &TileSet, const WorldSet &WorldSet, Storage<Transform> &transforms, const Entity target)
    : tilePainter(TilePainter),
    ScreenVector(ScreenSet.VirtualVector),
    TileSize(TileSet.TileSize),
    WorldSize(WorldSet.WorldSize),
    WorldMap(WorldSet.WorldMap),
    transforms(transforms),
    target(target) {
    }

    void Render(VirtualScreen &vs) override {
        Vector2 camera = transforms.Get(target)->position;

        Vector2 topLeft = camera;
        Vector2 bottomRight = topLeft + ScreenVector - Vector2::one();

        Vector2 first = topLeft / TileSize;
        Vector2 last = bottomRight / TileSize;

        for (int y = first.y; y <= last.y; y++) {
            for (int x = first.x; x <= last.x; x++) {
                if (x < 0 || x >= WorldSize.x) continue;
                if (y < 0 || y >= WorldSize.y) continue;

                tilePainter.DrawTileOnGrid(vs, Vector2(x, y), WorldMap[y*WorldSize.x + x]);
            }
        }
    }

private:
    TilePainter &tilePainter;
    Vector2 ScreenVector;
    Vector2 TileSize;

    Vector2 WorldSize;
    uint16_t *WorldMap;
    Storage<Transform> &transforms;
    const Entity target;
};
