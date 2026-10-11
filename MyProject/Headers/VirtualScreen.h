#pragma once

#include <vector>
#include <algorithm>

#include "BasicStructures.h"
#include "Object.h"
#include "TileSystem.h"

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
    VirtualScreen(const ScreenSet &ScreenSet, const TileSet &TileSet, Transform &target)
    : VirtualVector(ScreenSet.VirtualVector),
    HalfVirtualVector(VirtualVector / 2),
    ScreenBuffer(VirtualVector.y * VirtualVector.x, 0),

    target(target),
    TileSize(TileSet.TileSize),
    Tiles(TileSet.Tiles){}

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
        Vector2 lookVector = wv - target.position;

        if (!CheckCamBoundery(lookVector)) return;

        int pos = (tileData >> 8) * TileSize.x * TileSize.y;

        for (int y = 0; y < TileSize.y; ++y) {
            for (int x = 0; x < TileSize.x; ++x) {
                int tileNumber =    Tiles[pos + y*TileSize.x + x];
                int tilePalette =   (tileData >> 4) & 0x07;
                int tileLayer =     (tileData >> 2) & 0x03;

                Vector2 pxVector = Vector2(x, y);

                if (tileData & 0x02)
                    pxVector.x = (TileSize.x-1) - pxVector.x;
                if (tileData & 0x01)
                    pxVector.y = (TileSize.y-1) - pxVector.y;

                DrawPixel(
                    lookVector + HalfVirtualVector + pxVector,
                    PIXEL_NUMBER(tileNumber) | PIXEL_PALETTE(tilePalette) | PIXEL_LAYER(tileLayer)
                    );
            }
        }
    }

    void DrawTileOnGrid(Vector2 gv, uint16_t tileData) {
        Vector2 wv = gv.Scale(TileSize);

        DrawTileOnWorld(wv, tileData);
    }


    bool CheckCamBoundery(Vector2 wLookVector) {

        if (wLookVector.x - TileSize.x >= HalfVirtualVector.x || wLookVector.x + TileSize.x <  -HalfVirtualVector.x) return false;
        if (wLookVector.y - TileSize.y >= HalfVirtualVector.y || wLookVector.y + TileSize.y <  -HalfVirtualVector.y) return false;

        return true;
    }

private:
    void DrawPixel(Vector2 wv, uint8_t pixelData) {
        uint8_t typeMask = 0b00000011;
        int pos = wv.y * VirtualVector.x + wv.x;

        if (wv.x < 0 || wv.x >= VirtualVector.x)
            return;
        if (wv.y < 0 || wv.y >= VirtualVector.y)
            return;

        if ((ScreenBuffer[pos] & typeMask) > (pixelData & typeMask)) // isLayer Low?
            return;

        if (enableAlpha &&  (pixelData & 0b11100000) == 0)
            return;

        ScreenBuffer[pos] = pixelData;
    }

    bool enableAlpha = false;

    Vector2 VirtualVector;
    Vector2 HalfVirtualVector;

    //TILE:
    //ID 8, palette 3, layer 2, x-ivrt 1, y-ivrt 1
    //PIXEL:
    //color 3, palette 3, layer 2
    std::vector<uint8_t> ScreenBuffer;

    Transform &target;
    Vector2 TileSize;

    uint8_t *Tiles;
};
