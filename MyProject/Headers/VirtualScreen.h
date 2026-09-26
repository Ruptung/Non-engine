#pragma once

#include <vector>
#include <stdint.h>

#include "BasicStructures.h"
#include "IRule.h"

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
    VirtualScreen(Vector2 VirtualVector,Transform &cam, int TileSize, uint8_t *Tiles)
    : VirtualVector(VirtualVector), HalfVirtualVector(VirtualVector / 2), ScreenBuffer(VirtualVector.y * VirtualVector.x, 0), cam(cam), TileSize(TileSize), Tiles(Tiles){}

    const std::vector<uint8_t> &GetScreen() {
        return ScreenBuffer;
    }

    void Clear() {
        for (int i = 0; i < ScreenBuffer.size(); i++) {
            ScreenBuffer[i] = 0x00;
        }
    }

    void Alpha() {
        enableAlpha = true;
    }

    void DrawTileOnWorld(Vector2 wv, uint16_t tileData) {
        Vector2 lookVector = wv - cam.position * TileSize;

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
                    lookVector + HalfVirtualVector + pxVector,
                    PIXEL_NUMBER(tileNumber) | PIXEL_PALETTE(tilePalette) | PIXEL_LAYER(tileLayer)
                    );
            }
        }
    }

    void DrawTileOnGrid(Vector2 gv, uint16_t tileData) {
        Vector2 lookVector = (gv - cam.position) * TileSize;

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
                    lookVector + HalfVirtualVector + pxVector,
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
    int TileSize;
    Vector2 VirtualVector;

    Vector2 HalfVirtualVector;

    Transform &cam;
    //color 3, palette 3, layer 2
    std::vector<uint8_t> ScreenBuffer;

    uint8_t *Tiles;
};
