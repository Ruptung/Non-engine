#pragma once

#include <cstdint>
#include <vector>

#include "BasicStructures.h"

class Painter {
public:
    Painter(Vector2 VirtualVector, uint32_t *Palette)
    : VitrualVector(VirtualVector),
    screen(VirtualVector.y * VirtualVector.x),
    palette(Palette)
    {}

    const std::vector<uint32_t>* GetScreen(const std::vector<uint8_t> &virtualScreen) {
        for (int vy = 0; vy < VitrualVector.y; vy++) {
            for (int vx = 0; vx < VitrualVector.x; vx++) {
                uint8_t data = virtualScreen[vy * VitrualVector.x + vx];

                //get palette and color
                uint32_t rgbColor = palette[((data & 0b00011100) >> 2) * 8 + ((data & 0b11100000) >> 5)];

                screen[vy * VitrualVector.x + vx] = rgbColor;
            }
        }

        return &screen;
    }

    void clearScreen() {
        for (int vy = 0; vy < VitrualVector.y; vy++) {
            for (int vx = 0; vx < VitrualVector.x; vx++) {
                screen[vy * VitrualVector.x + vx] = 0x00;
            }
        }
    }
private:
    uint32_t *palette;

    Vector2 VitrualVector;
    std::vector<uint32_t> screen;
};
