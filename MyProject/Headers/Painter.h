#pragma once
#include <cstdint>
#include <vector>

class Painter {
public:
    Painter(int virtualHeight, int virtualWidth, uint32_t *Palette)
    : VirtualHeight(virtualHeight), VirtualWidth(virtualWidth),
    screen(virtualHeight * virtualWidth),
    palette(Palette)
    {}

    const std::vector<uint32_t>* GetScreen(const std::vector<uint8_t> virtualScreen) {
        for (int vy = 0; vy < VirtualHeight; vy++) {
            for (int vx = 0; vx < VirtualWidth; vx++) {
                uint8_t data = virtualScreen[vy * VirtualWidth + vx];

                //get palette and color
                uint32_t rgbColor = palette[((data & 0b00011100) >> 2) * 8 + ((data & 0b11100000) >> 5)];

                screen[vy * VirtualWidth + vx] = rgbColor;
            }
        }

        return &screen;
    }
private:
    uint32_t *palette;

    int VirtualHeight, VirtualWidth;
    std::vector<uint32_t> screen;
};
