#pragma once

#include "Headers/Interfaces/IRule.h"

#include "Headers/BasicStructures.h"
#include "Headers/Object.h"
#include "Headers/Storage.h"
#include "Headers/World.h"

class ColliderRule : public IRule {
private:
    Storage<Collider> &colliders;
    Storage<Transform> &transforms;
    World &world;
public:
    ColliderRule(Storage<Collider> &colliders, Storage<Transform> &transforms, World &world)
    : colliders(colliders),
    transforms(transforms),
    world(world){
    }
    //~ColliderRule() = default;
    void Update() override {
        auto& dense_collider = colliders.All();
        auto& owner_collider = colliders.Owners();

        for (size_t object_idx = 0; object_idx < dense_collider.size(); object_idx++) {
            for (size_t target_idx = object_idx+1; target_idx < dense_collider.size(); target_idx++) {
                Entity object = owner_collider[object_idx];
                Entity target = owner_collider[target_idx];

                if (CheckBoxCollide(*transforms.Get(object), *transforms.Get(target), 1)) {

                    std::vector<ICollide*> a = world.FindAll<ICollide>(object);
                    std::vector<ICollide*> b = world.FindAll<ICollide>(target);

                    for (ICollide *collide: a) {
                        collide->Collide(target);
                    }
                    for (ICollide *collide: b) {
                        collide->Collide(object);
                    }
                }
            }
        }
    }

    bool CheckCollide(const Transform &origin, const Transform &moveto) {
        auto& dense_collider = colliders.All();
        auto& owner_collider = colliders.Owners();

        for (size_t object_idx = 0; object_idx < dense_collider.size(); object_idx++) {
            Entity object = owner_collider[object_idx];
            Transform &t = *transforms.Get(object);

            if (&t == &origin) {
                continue;
            }

            if (CheckBoxCollide(t, moveto, 0)) {
                return true;
            }
        }

        return false;
    }
private:
    bool CheckBoxCollide(const Transform &object, const Transform &target, int offset) {
        Vector2Int object_pos = object.position;
        Vector2Int target_pos = target.position;
        Vector2Int object_ext = object.extend;
        Vector2Int target_ext = target.extend;


        return (target_pos.x <= object_pos.x + object_ext.x + offset) && (object_pos.x <= target_pos.x + target_ext.x + offset) &&
            (target_pos.y <= object_pos.y + object_ext.y + offset) && (object_pos.y <= target_pos.y + target_ext.y + offset);
    }
};
