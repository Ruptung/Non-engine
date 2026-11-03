#pragma once

#include "Headers/Object.h"
#include "Headers/Storage.h"
#include "Headers/Addons/Tile_Addon/TileSystem.h"
#include "Headers/Interfaces/IRule.h"
#include "Implements/PersonalScript/PlayerMoveScript.h"

class PhysicsRule : public IRule {
private:
    Storage<Physics> &physics;
    Storage<Transform> &transforms;
    Vector2 WorldSize;
    uint16_t *WorldSpec;
    Vector2 TileSize;

public:
    PhysicsRule(Storage<Physics> &physics, Storage<Transform> &transforms, const WorldSet &WorldSet, const TileSet TileSet)
    : physics(physics),
    transforms(transforms),
    WorldSize(WorldSet.WorldSize),
    WorldSpec(WorldSet.WorldSpec),
    TileSize(TileSet.TileSize)
    {

    }

    // Not gonna implement Accel now.
    void Update() override {
        std::vector<Physics> &dense = physics.All();
        const std::vector<Entity> &owner = physics.Owners();


        for (size_t i = 0; i < dense.size(); i++) {
            const Vector2 &velocity = dense[i].Velocity;

            if (velocity.x == 0 && velocity.y == 0) { continue; }

            Transform &transform = transforms.Get(owner[i]);

            transform.position = MoveTo(transform.position, velocity, transform.scale);

            dense[i].Velocity = Vector2::zero();
        }
    }
private:
    Vector2 MoveTo(Vector2 pos, Vector2 moveto, Vector2 scale) {
        if (scale.x == 0 || scale.y == 0) { return pos + moveto; }
        else { scale = scale - Vector2::one(); }

        int iter = (moveto.x == 0) ? abs(moveto.y) : (moveto.y == 0) ? abs(moveto.x) : std::min(abs(moveto.x), abs(moveto.y)); // min
        Vector2 ratio_move = moveto / iter;

        for (int i = 0; i < iter; i++) {
            if (!CanMove(pos, ratio_move, scale)) { break; }

            pos += ratio_move;
        }
        return pos;
    }
    bool CanMove(Vector2 pos, Vector2 moveto, Vector2 scale) {
        Vector2 pivot1 = pos + moveto;
        Vector2 pivot2 = pivot1 + scale;

        if (moveto.x > 0) {
            pivot1.x += scale.x;
        }else if (moveto.x < 0) {
            pivot2.x -= scale.x;
        }

        if (moveto.y > 0) {
            pivot1.y += scale.y;
        }else if (moveto.y < 0) {
            pivot2.y -= scale.y;
        }

        Vector2 gp1 = pivot1 / TileSize;
        Vector2 gp2 = pivot2 / TileSize;

        //TODO Make this divide by 4 to Grid/Tile Size.
        int flatten1 = gp1.y * WorldSize.x + gp1.x;
        int flatten2 = gp2.y * WorldSize.x + gp2.x;

        if ((gp1.x >= 0 && gp1.x  < WorldSize.x) && (gp1.y >= 0 && gp1.y < WorldSize.y)) {
            if (WorldSpec[flatten1] & FLAG_SOLID) {
                return false;
            }
        }
        if ((gp2.x >= 0 && gp2.x < WorldSize.x) && (gp2.y >= 0 && gp2.y < WorldSize.y)) {
            if (WorldSpec[flatten2] & FLAG_SOLID) {
                return false;
            }
        }
        return true;
    }
};
