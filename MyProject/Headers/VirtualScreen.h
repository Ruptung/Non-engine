#pragma once

#include <vector>
#include <algorithm>

#include "BasicStructures.h"

#define PIXEL_NUMBER(num)   ((num) << 5)
#define PIXEL_PALETTE(num)  ((num) << 2)
#define PIXEL_LAYER(layer)  (layer)

class VirtualScreen {
public:
    VirtualScreen(const ScreenSet &ScreenSet)
    : VirtualVector(ScreenSet.VirtualVector),
    HalfVirtualVector(VirtualVector / 2),
    ScreenBuffer(VirtualVector.y * VirtualVector.x, 0) {
    }

    const std::vector<uint8_t> &GetScreen() {
        return ScreenBuffer;
    }

    void Clear() {
        std::fill(ScreenBuffer.begin(), ScreenBuffer.end(), 0);
    }

    void Alpha() {
        enableAlpha = true;
    }

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

    bool CheckCamBoundery(Vector2 wLookVector, Vector2 size) {

        if (wLookVector.x + size.x <= 0 || wLookVector.x >= VirtualVector.x) return false;
        if (wLookVector.y + size.y <= 0 || wLookVector.y >= VirtualVector.y) return false;

        return true;
    }
private:
    bool enableAlpha = false;

    Vector2 VirtualVector;
    Vector2 HalfVirtualVector;
    //TILE:
    //ID 8, palette 3, layer 2, x-ivrt 1, y-ivrt 1
    //PIXEL:
    //color 3, palette 3, layer 2
    std::vector<uint8_t> ScreenBuffer;
};
