#pragma once

#include "Headers/Interfaces/IRender.h"
#include "Headers/VirtualScreen.h"

class TileObjectRender : public IRender {
public:
    TileObjectRender (TilePainter &TilePainter, Storage<Renderable> &renderables, Storage<Transform> &transforms)
    : tilePainter(TilePainter),
    renderables(renderables),
    transforms(transforms) {
    }

    //TODO cull by chunk.
    void Render(VirtualScreen &vs) override{
        const std::vector<Entity> &own_renderables = renderables.Owners();
        const std::vector<Renderable> &dense_renderables = renderables.All();

        for (size_t i = 0; i < dense_renderables.size(); i++) {
            tilePainter.DrawTileOnWorld(vs, transforms.Get(own_renderables[i])->position, dense_renderables[i].tileData);
        }
    }
private:
    TilePainter &tilePainter;
    Storage<Renderable>& renderables;
    Storage<Transform>& transforms;
};
