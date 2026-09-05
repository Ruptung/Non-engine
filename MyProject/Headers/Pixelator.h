#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>
#include <iostream>

#include "BasicStructurs.h"


class Pixelator {
public:
    Pixelator(int ) : frameBuffer(REAL_WIDTH* REAL_HEIGHT, 0xFF000000) {}

    void DrawVirtual2Real(int y, int x, uint32_t color) {
        int px = x * RATIO;
        int py = y * RATIO;

        for (int i = 0; i < RATIO; i++) {
            for (int j = 0; j < RATIO; j++) {
                PutPixel(px + i, py + j, color);
            }
        }
    }

    std::vector<uint32_t>* getBuffer() {
        return &frameBuffer;
    }

private:
    std::vector<uint32_t> frameBuffer;

    void PutPixel(int x, int y, uint32_t color) {
        if (x < 0 || x >= REAL_WIDTH || y < 0 || y >= REAL_HEIGHT) return;

        frameBuffer[y * REAL_WIDTH + x] = color;
    }
};

