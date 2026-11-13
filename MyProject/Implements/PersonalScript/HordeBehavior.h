#pragma once
#include "Headers/Object.h"
#include "Headers/Interfaces/IScript.h"

class HordeBehavior : public IScript {
private:
    Entity target;
    Storage<Transform> *transforms;
    Storage<Physics> *physics;
    Storage<Zombie> *zombies;
    int frame;
public:
    HordeBehavior(Entity target, Storage<Transform> *transforms, Storage<Physics> *physics, Storage<Zombie> *zombies)
    : target(target),
    transforms(transforms),
    physics(physics),
    zombies(zombies),
    frame(0)
    {
    }

    void Start() override {

    }
    void Update() override {
        frame++;
        if ((frame %= 10) != 0) return;


        auto &zombie_dense = zombies->All();
        auto &zombie_owner = zombies->Owners();

        Transform *tf = transforms->TryGet(target);
        if (!tf) { return; }

        Vector2Int p_pos = transforms->Get(target)->position;


        for (size_t i = 0; i < zombie_dense.size(); i++) {

            const Vector2Int *z_pos = &transforms->Get(zombie_owner[i])->position;

            physics->Get(zombie_owner[i])->Velocity = Vector2Int(
                (p_pos.x < z_pos->x) ? -1 : (p_pos.x > z_pos->x) ? 1 : 0,
                (p_pos.y < z_pos->y) ? -1 : (p_pos.y > z_pos->y) ? 1 : 0
                );

            //printf("%d", zombie);
        }
    }
};