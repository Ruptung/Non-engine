#pragma once
#include "Headers/Object.h"
#include "Headers/Interfaces/IScript.h"

class HordeBehavior : public IScript {
private:
    Entity target;
    Storage<Transform> *transforms;
    Storage<Physics> *physics;
    Storage<Zombie> *zombies;
public:
    HordeBehavior(Entity target, Storage<Transform> *transforms, Storage<Physics> *physics, Storage<Zombie> *zombies)
    : target(target),
    transforms(transforms),
    physics(physics),
    zombies(zombies)
    {
    }

    void Start() override {

    }
    void Update() override {
        auto &zombie_dense = zombies->All();
        auto &zombie_owner = zombies->Owners();

        Transform *tf = transforms->TryGet(target);
        if (!tf) { return; }

        Vector2 p_pos = transforms->Get(target)->position;


        for (size_t i = 0; i < zombie_dense.size(); i++) {
            const Vector2 *z_pos = &transforms->Get(zombie_owner[i])->position;

            Vector2 lookVec = (p_pos - *z_pos);

            physics->Get(zombie_owner[i])->Velocity = (lookVec.magnitude() > 0.2f) ? lookVec.normalize() * 0.2f : lookVec;

            //printf("%d", zombie);
        }
    }
};