#pragma once

#include "../../BasicStructures.h"

struct TileSet {
    TileSet(Vector2 TileSize, uint8_t *Tiles)
    : TileSize(TileSize),
    Tiles(Tiles) {
    }

    Vector2 TileSize;
    uint8_t *Tiles;
};
struct WorldSet {
    WorldSet(Vector2 WorldSize, uint16_t *WorldMap, uint16_t *WorldSpec) :
    WorldSize(WorldSize),
    WorldMap(WorldMap),
    WorldSpec(WorldSpec) {
    }

    Vector2 WorldSize;
    uint16_t *WorldMap;
    uint16_t *WorldSpec;
};
