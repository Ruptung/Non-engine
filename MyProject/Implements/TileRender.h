#pragma once

#include "../Headers/IRender.h"
#include "../Headers/VirtualScreen.h"

class TileRender : public IRender {
public:
    TileRender (std::vector<Renderable*> &renderables) : renderables(renderables) {
    }

    void Render(VirtualScreen &vs) override{
        for (Renderable *r : renderables)
            vs.DrawTileOnGrid(r->transform->position, r->tileData);
    }
private:
    std::vector<Renderable*>& renderables;
};