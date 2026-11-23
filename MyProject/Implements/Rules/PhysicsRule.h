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
    ColliderRule &colliderRule;
    Vector2Int WorldSize;
    uint16_t *WorldSpec;
    Vector2Int TileSize;


public:
    PhysicsRule(Storage<Physics> &physics, Storage<Transform> &transforms, ColliderRule &colliderRule, const WorldSet &WorldSet, const TileSet TileSet)
    : physics(physics),
    transforms(transforms),
    colliderRule(colliderRule),
    WorldSize(WorldSet.WorldSize),
    WorldSpec(WorldSet.WorldSpec),
    TileSize(TileSet.TileSize)
    {

    }

    void Update() override {
        std::vector<Physics> &dense = physics.All();
        const std::vector<Entity> &owner = physics.Owners();


        for (size_t i = 0; i < dense.size(); i++) {
            const Vector2 &velocity = dense[i].Velocity;

            if (velocity.x == 0 && velocity.y == 0) { continue; }

            Transform &transform = *transforms.Get(owner[i]);

            AppendVelocity(transform, velocity);
            dense[i].Velocity = Vector2::zero();
        }
    }
private:
    void MoveStep(Transform &tf, Vector2 norm) {
        Vector2 x_axis_move = {norm.x, 0};
        if (CanMove(tf, x_axis_move)) {
            tf.position = tf.position + x_axis_move;
        }

        Vector2 y_axis_move = {0, norm.y};
        if (CanMove(tf, y_axis_move)) {
            tf.position = tf.position + y_axis_move;
        }
    }

    void AppendVelocity(Transform &tf, const Vector2 velocity) {
        if (velocity.x == 0 && velocity.y == 0) { return; }

        bool XorY = velocity.x == 0;

        Vector2 norm = velocity.normalize();
        float step = std::abs(XorY ? norm.y : norm.x);
        norm = norm / step;

        float iter = std::abs(XorY ? velocity.y : velocity.x);

        while (iter > 1) {
            MoveStep(tf, norm);

            iter += -1;
        }

        Vector2 left_move = {norm * iter};
        MoveStep(tf, left_move);
    }

    bool CanMove(const Transform &tf, const Vector2 relative_position) {
        Vector2 next = tf.position + relative_position;
        Vector2 scale = tf.extend;

        if (colliderRule.CheckCollide(tf, Transform{next, scale})) {
            return false;
        }

        Vector2 pivot1 = next;
        Vector2 pivot2 = pivot1 + scale;

        if (relative_position.x > 0) {
            pivot1.x += scale.x;
        }else if (relative_position.x < 0) {
            pivot2.x -= scale.x;
        }

        if (relative_position.y > 0) {
            pivot1.y += scale.y;
        }else if (relative_position.y < 0) {
            pivot2.y -= scale.y;
        }

        Vector2Int gp1 = pivot1 / TileSize;

        if ((gp1.x >= 0 && gp1.x  < WorldSize.x) && (gp1.y >= 0 && gp1.y < WorldSize.y)) {
            int flatten1 = gp1.y * WorldSize.x + gp1.x;

            if (WorldSpec[flatten1] & FLAG_SOLID) {
                return false;
            }
        }

        Vector2Int gp2 = pivot2 / TileSize;

        if ((gp2.x >= 0 && gp2.x < WorldSize.x) && (gp2.y >= 0 && gp2.y < WorldSize.y)) {
            int flatten2 = gp2.y * WorldSize.x + gp2.x;

            if (WorldSpec[flatten2] & FLAG_SOLID) {
                return false;
            }
        }
        return true;
    }
};
