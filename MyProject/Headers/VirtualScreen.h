#pragma once

#include <vector>
#include <stdint.h>

#include "BasicStructures.h"
#include "IRule.h"
#include "Object.h"
#include "Storage.h"

#define TILE_ID(id)         ((id) << 8)
#define TILE_PALETTE(num)   ((num) << 4)
#define TILE_LAYER(layer)   ((layer) << 2)
#define TILE_X_IVRT         (1 << 1)
#define TILE_Y_IVRT         (1)

#define PIXEL_NUMBER(num)   ((num) << 5)
#define PIXEL_PALETTE(num)  ((num) << 2)
#define PIXEL_LAYER(layer)  (layer)

class VirtualScreen {
public:
    VirtualScreen(Vector2 VirtualVector, Storage<Transform> &objects, const Entity target, int TileSize, uint8_t *Tiles)
    : VirtualVector(VirtualVector),
    HalfVirtualVector(VirtualVector / 2),
    ScreenBuffer(VirtualVector.y * VirtualVector.x, 0),
    objects(objects),
    target(target),
    TileSize(TileSize),
    Tiles(Tiles){}

    const std::vector<uint8_t> &GetScreen() {
        return ScreenBuffer;
    }

    void Clear() {
        std::fill(ScreenBuffer.begin(), ScreenBuffer.end(), 0);
    }

    void Alpha() {
        enableAlpha = true;
    }

    void DrawTileOnWorld(Vector2 wv, uint16_t tileData) {
        Vector2 lookVector = wv - objects.Get(target).position * TileSize;

        if (!CheckCamBoundery(lookVector)) return;

        int pos = (tileData >> 8) *TileSize*TileSize;
        for (int y = 0; y < TileSize; ++y) {
            for (int x = 0; x < TileSize; ++x) {
                int tileNumber =    Tiles[pos + y * TileSize + x];
                int tilePalette =   (tileData >> 4) & 0x07;
                int tileLayer =     (tileData >> 2) & 0x03;

                Vector2 pxVector = Vector2(x, y);

                if (tileData & 0x02)
                    pxVector.x = (TileSize-1) - pxVector.x;
                if (tileData & 0x01)
                    pxVector.y = (TileSize-1) - pxVector.y;

                DrawPixel(
                    lookVector + HalfVirtualVector + pxVector - Vector2(2, 2),
                    PIXEL_NUMBER(tileNumber) | PIXEL_PALETTE(tilePalette) | PIXEL_LAYER(tileLayer)
                    );
            }
        }
    }

    void DrawTileOnGrid(Vector2 gv, uint16_t tileData) {
        Vector2 lookVector = (gv - objects.Get(target).position) * TileSize;

        if (!CheckCamBoundery(lookVector)) return;

        int pos = (tileData >> 8) *TileSize*TileSize;
        for (int y = 0; y < TileSize; ++y) {
            for (int x = 0; x < TileSize; ++x) {
                int tileNumber =    Tiles[pos + y * TileSize + x];
                int tilePalette =   (tileData >> 4) & 0x07;
                int tileLayer =     (tileData >> 2) & 0x03;


                Vector2 pxVector = Vector2(x, y);

                if (tileData & 0x02)
                    pxVector.x = (TileSize-1)- pxVector.x;
                if (tileData & 0x01)
                    pxVector.y = (TileSize-1) - pxVector.y;

                DrawPixel(
                    lookVector + HalfVirtualVector + pxVector - Vector2(2, 2),
                    PIXEL_NUMBER(tileNumber) | PIXEL_PALETTE(tilePalette) | PIXEL_LAYER(tileLayer)
                    );
            }
        }
    }


    bool CheckCamBoundery(Vector2 wLookVector) {

        if (wLookVector.x >= HalfVirtualVector.x || wLookVector.x <  - HalfVirtualVector.x) return false;
        if (wLookVector.y >= HalfVirtualVector.y || wLookVector.y <  - HalfVirtualVector.y) return false;

        return true;
    }

private:
    void DrawPixel(Vector2 wv, uint8_t pixelData) {
        uint8_t typeMask = 0b00000011;
        int pos = wv.y * VirtualVector.x + wv.x;

        if ((ScreenBuffer[pos] & typeMask) > (pixelData & typeMask)) // isLayer Low?
            return;

        if (enableAlpha &&  (pixelData & 0b11100000) == 0)
            return;

        ScreenBuffer[pos] = pixelData;
    }
    //TILE:
    //ID 8, palette 3, layer 2, x-ivrt 1, y-ivrt 1

    bool enableAlpha = false;

    Vector2 VirtualVector;
    Vector2 HalfVirtualVector;

    std::vector<uint8_t> ScreenBuffer;

    Storage<Transform> &objects;
    const Entity target;

    //color 3, palette 3, layer 2

    int TileSize;

    uint8_t *Tiles;
};