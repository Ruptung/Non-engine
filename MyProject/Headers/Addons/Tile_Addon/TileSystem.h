#pragma once

#include "../../BasicStructures.h"

struct TileSet {
    TileSet(Vector2Int TileSize, uint8_t *Tiles)
    : TileSize(TileSize),
    Tiles(Tiles) {
    }

    Vector2Int TileSize;
    uint8_t *Tiles;
};
struct WorldSet {
    WorldSet(Vector2Int WorldSize, uint16_t *WorldMap, uint16_t *WorldSpec) :
    WorldSize(WorldSize),
    WorldMap(WorldMap),
    WorldSpec(WorldSpec) {
    }

    Vector2Int WorldSize;
    uint16_t *WorldMap;
    uint16_t *WorldSpec;
};
