#pragma once
#include <cstdint>
#include <vector>


class VirtualScreen {
public:
    VirtualScreen(int vHeight, int vWidth)
    : VirtualHeight(vHeight), VirtualWidth(vWidth), ScreenBuffer(vHeight * vWidth, 0){}

    std::vector<uint8_t>* GetScreen() {
        return &ScreenBuffer;
    }

    int SetTiles(uint8_t *tiles, int tileSize) {
        TileSize = tileSize;

        if (sizeof(tiles) / sizeof(uint8_t) > tileSize * tileSize * 256)
            return -1;

        Tiles = tiles;

        return 0;
    }

    void DrawTileOnWorld(int wy, int wx, uint16_t tileData) {
        int pos = (tileData >> 8) *TileSize*TileSize;

        for (int y = 0; y < TileSize; ++y) {
            for (int x = 0; x < TileSize; ++x) {
                DrawPixel(wy+y, wx+x, Tiles[pos + y * TileSize + x]);
            }
        }
    }

    void DrawTileOnGrid(int gy, int gx, uint16_t tileData) {
        DrawTileOnWorld(gy * TileSize, gx * TileSize, tileData);
    }

private:
    void DrawPixel(int wy, int wx, uint8_t pixelData) {
        uint8_t typeMask = 0b00000011;
        int pos = wy * VirtualWidth + wx;

        if ((ScreenBuffer[pos] & typeMask) > (pixelData & typeMask)) // isLayer Low?
            return;

        ScreenBuffer[pos] = pixelData;
    }
    //TILE:
    //ID 8, palette 3, layer 2, y-ivrt 1, x-ivrt 1


    int TileSize;
    int VirtualHeight, VirtualWidth;
    //color 3, palette 3, layer 2
    std::vector<uint8_t> ScreenBuffer;

    uint8_t *Tiles;
};
