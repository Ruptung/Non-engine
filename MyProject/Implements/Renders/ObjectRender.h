#pragma once

#include "Headers/IRender.h"
#include "Headers/VirtualScreen.h"

class ObjectRender : public IRender {
public:
    ObjectRender (Storage<Renderable> &renderables, Storage<Transform> &transforms)
    : renderables(renderables), transforms(transforms) {
    }

    void Render(VirtualScreen &vs) override{
        const std::vector<Entity> &own_renderables = renderables.Owners();
        const std::vector<Renderable> &dense_renderables = renderables.All();

        for (int i = 0; i < static_cast<int>(dense_renderables.size()); i++) {
            vs.DrawTileOnGrid(transforms.Get(own_renderables[i]).position, dense_renderables[i].tileData);
        }
    }
private:
    Storage<Renderable>& renderables;
    Storage<Transform>& transforms;
};
