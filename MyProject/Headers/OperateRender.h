#pragma once

#include "SDL_Wizard.h"
#include "IOperator.h"
#include "Painter.h"
#include "VirtualScreen.h"

class OperateRender : public IOperator{
public:
    OperateRender(const int vH, const int vW, const int sH, const int sW, uint32_t *Palette, uint8_t *Tiles)
    : wiz(vH, vW, sH, sW), vs(vH, vW, 4, Tiles), painter(vH, vW, Palette) {
    }

    void Update() override {
        vs.DrawTileOnGrid(0, 0, TILE_ID(0) | TILE_LAYER(1) | TILE_PALETTE(0));

        vs.DrawTileOnGrid(1, 1, TILE_ID(0) | TILE_LAYER(1) | TILE_PALETTE(0));

        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));
    }
private:
    SDL_Wizard wiz;
    VirtualScreen vs;
    Painter painter;
};