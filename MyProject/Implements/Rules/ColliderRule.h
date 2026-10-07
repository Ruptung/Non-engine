#pragma once

#include "Headers/BasicStructures.h"
#include "Headers/IRule.h"
#include "Headers/Object.h"
#include "Headers/Storage.h"
#include "Headers/World.h"

class ColliderRule : public IRule {
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

                if (CheckBoxCollide(transforms.Get(object), transforms.Get(target))) {

                    std::vector<ICollide*> a = world.GetCollides(object);
                    std::vector<ICollide*> b = world.GetCollides(target);

                    for (ICollide * collide: a) {
                        collide->Collide(target);
                    }
                    for (ICollide * collide: b) {
                        collide->Collide(object);
                    }
                }
            }
        }
    }
private:
    bool CheckBoxCollide(Transform object, Transform target) {
        Vector2 object_pos = object.position;
        Vector2 target_pos = target.position;

        if (object_pos.x + 3 < target_pos.x || object_pos.x > target_pos.x + 3) { return false; }
        if (object_pos.y + 3 < target_pos.y || object_pos.y > target_pos.y + 3) { return false; }

        return true;
    }

    Storage<Collider> &colliders;
    Storage<Transform> &transforms;
    World &world;
};
