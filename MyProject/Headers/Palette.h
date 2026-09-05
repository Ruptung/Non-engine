#pragma once
#include <cstdint>
#include "Pixelator.h"

#define TILE_SIZE 4

uint8_t tiles[3][4][4] = {
    {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,1,1,0},
        {1,0,0,1},
        {1,0,0,1},
        {0,1,1,0},
    },
    {
        {2,0,0,2},
        {0,2,2,0},
        {0,2,2,0},
        {2,0,0,2},
    }
};

uint32_t colors[] = {
    0xFF000000,
    0xFFFF0000,
    0xFF00FF00
};

void DrawPalette(Pixelator &pixelator, unsigned char tile, int posY, int posX){
    for (int ty = 0; ty < TILE_SIZE; ty++) {
        uint8_t* line = tiles[tile][ty];

        for (int tx = 0; tx < TILE_SIZE; tx++) {
            pixelator.DrawVirtual2Real(
                posY * TILE_SIZE + ty + posY + 1,
                posX * TILE_SIZE + tx + posX + 1,
                colors[line[tx]]
            );
        }
    }
}