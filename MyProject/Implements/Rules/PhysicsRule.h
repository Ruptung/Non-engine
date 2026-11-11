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
    Vector2Int WorldSize;
    uint16_t *WorldSpec;
    Vector2Int TileSize;

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
            const Vector2Int &velocity = dense[i].Velocity;

            if (velocity.x == 0 && velocity.y == 0) { continue; }

            Transform &transform = *transforms.Get(owner[i]);

            transform.position = AppendVelocity(transform.position, velocity, transform.scale);

            dense[i].Velocity = Vector2Int::zero();
        }
    }
private:
    Vector2Int AppendVelocity(Vector2Int pos, Vector2Int velocity, Vector2Int scale) {
        if (scale.x == 0 || scale.y == 0) { return pos + velocity; }
        else { scale = scale - Vector2Int::one(); }

        int iter = (velocity.x == 0) ? abs(velocity.y) : (velocity.y == 0) ? abs(velocity.x) : std::min(abs(velocity.x), abs(velocity.y)); // min
        Vector2Int ratio_move = velocity / iter;

        bool fx = false;
        bool fy = false;
        for (int i = 0; i < iter; i++) {
            if (fx == false && CanMove(pos, Vector2Int(ratio_move.x, 0), scale)) { pos.x += ratio_move.x; }
            else { fx = true; }
            if (fy == false && CanMove(pos, Vector2Int(0, ratio_move.y), scale)) { pos.y += ratio_move.y; }
            else { fy = true; }
        }

        return pos;
    }
    bool CanMove(Vector2Int pos, Vector2Int moveto, Vector2Int scale) {
        Vector2Int pivot1 = pos + moveto;
        Vector2Int pivot2 = pivot1 + scale;

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

        Vector2Int gp1 = pivot1 / TileSize;
        Vector2Int gp2 = pivot2 / TileSize;

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
