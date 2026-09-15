#pragma once

#pragma once

#include "../Headers/IRender.h"
#include "../Headers/VirtualScreen.h"

class TileRender : public IRender {
public:
    void Render(VirtualScreen &vs) override{
        vs.DrawTileOnGrid(0, 0, TILE_ID(0) | TILE_LAYER(1) | TILE_PALETTE(0));

        vs.DrawTileOnGrid(1, 1, TILE_ID(0) | TILE_LAYER(1) | TILE_PALETTE(0));
    }
};