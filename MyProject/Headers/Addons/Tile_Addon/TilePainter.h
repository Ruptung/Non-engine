#pragma once

#include "Headers/BasicStructures.h"
#include "Headers/Object.h"
#include "Headers/Addons/Tile_Addon/TileSystem.h"
#include "Headers/VirtualScreen.h"

#define TILE_ID(id)         ((id) << 8)
#define TILE_PALETTE(num)   ((num) << 4)
#define TILE_LAYER(layer)   ((layer) << 2)
#define TILE_X_IVRT         (1 << 1)
#define TILE_Y_IVRT         (1)

//TILE:
    //ID 8, palette 3, layer 2, x-ivrt 1, y-ivrt 1

//PIXEL:
    //color 3, palette 3, layer 2

class TilePainter {
public:
    TilePainter(const TileSet &TileSet, Transform *target)
    : target(target),
    TileSize(TileSet.TileSize),
    Tiles(TileSet.Tiles){}

    void DrawTileOnWorld(VirtualScreen &VirtualScreen, Vector2Int wv, uint16_t tileData) {
        Vector2Int lookVector = wv - target->position;

        if (!VirtualScreen.CheckCamBoundery(lookVector, TileSize)) return;

        int pos = (tileData >> 8) * TileSize.x * TileSize.y;

        for (int y = 0; y < TileSize.y; ++y) {
            for (int x = 0; x < TileSize.x; ++x) {
                int tileNumber =    Tiles[pos + y*TileSize.x + x];
                int tilePalette =   (tileData >> 4) & 0x07;
                int tileLayer =     (tileData >> 2) & 0x03;

                Vector2Int pxVector = Vector2Int(x, y);

                if (tileData & 0x02)
                    pxVector.x = (TileSize.x-1) - pxVector.x;
                if (tileData & 0x01)
                    pxVector.y = (TileSize.y-1) - pxVector.y;

                VirtualScreen.DrawPixel(
                    lookVector + pxVector,
                    PIXEL_NUMBER(tileNumber) | PIXEL_PALETTE(tilePalette) | PIXEL_LAYER(tileLayer)
                    );
            }
        }
    }

    void DrawTileOnGrid(VirtualScreen &VirtualScreen, Vector2Int gv, uint16_t tileData) {
        Vector2Int wv = gv.Scale(TileSize);

        DrawTileOnWorld(VirtualScreen, wv, tileData);
    }


private:
    Transform *target;

    Vector2Int TileSize;
    uint8_t *Tiles;
};
