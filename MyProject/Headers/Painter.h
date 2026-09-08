#pragma once
#include <cstdint>
#include <vector>

class Painter {
public:
    Painter(int screenHeight, int screenWidth, int virtualHeight, int virtualWidth)
    : ScreenHeight(screenHeight), ScreenWidth(screenWidth), VirtualHeight(virtualHeight), VirtualWidth(virtualWidth),
    Ratio(screenHeight / virtualHeight), screen(screenHeight * screenWidth, 0) {}

    const std::vector<uint32_t>* GetScreen(const std::vector<uint8_t> &virtualScreen) {
        for (int vy = 0; vy < VirtualHeight; vy++) {
            for (int vx = 0; vx < VirtualWidth; vx++) {
                uint8_t data = virtualScreen[vy * VirtualWidth + vx];

                //get palette and color
                uint32_t rgbColor = palette[((data & 0b00011100) >> 2 * 8) + ((data & 0b11100000) >> 5)];

                for (int ry = 0; ry < Ratio; ry++) {
                    for (int rx = 0; rx < Ratio; rx++) {
                        screen[((vy * Ratio + ry) * ScreenWidth) + (vx * Ratio + rx)] = rgbColor ;
                    }
                }
            }
        }

        return &screen;
    }

    void SetPalette(uint32_t *p) {
        palette = p;
    }
private:
    uint32_t *palette = nullptr;

    int ScreenHeight, ScreenWidth, VirtualHeight, VirtualWidth, Ratio;
    std::vector<uint32_t> screen;
};
