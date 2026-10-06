#pragma once

#include <cstdint>
#include <vector>

#include "BasicStructures.h"

class Painter {
public:
    Painter(Vector2 VirtualVector, uint32_t *Palette)
    : VirtualVector(VirtualVector),
    palette(Palette),
    screen(VirtualVector.y * VirtualVector.x, 0)
    {}

    const std::vector<uint32_t>* GetScreen(const std::vector<uint8_t> &virtualScreen) {
        for (int vy = 0; vy < VirtualVector.y; vy++) {
            for (int vx = 0; vx < VirtualVector.x; vx++) {
                uint8_t data = virtualScreen[vy * VirtualVector.x + vx];

                //get palette and color
                uint32_t rgbColor = palette[((data & 0b00011100) >> 2) * 8 + ((data & 0b11100000) >> 5)];

                screen[vy * VirtualVector.x + vx] = rgbColor;
            }
        }

        return &screen;
    }

    void clearScreen() {
        for (int vy = 0; vy < VirtualVector.y; vy++) {
            for (int vx = 0; vx < VirtualVector.x; vx++) {
                screen[vy * VirtualVector.x + vx] = 0x00;
            }
        }
    }
private:
    Vector2 VirtualVector;
    uint32_t *palette;
    std::vector<uint32_t> screen;
};
