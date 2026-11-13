#pragma once

#include "Headers/World.h"
#include "Headers/Interfaces/IRule.h"

class CreatureRule : public IRule {
private:
    World *world;
    Storage<Creature> &creatures;
    std::vector<Creature> &dense_creatures;
    const std::vector<Entity> &owner_creatures;

public:
    CreatureRule(World *world, Storage<Creature> &creatures)
    : world(world),
    creatures(creatures),
    dense_creatures(creatures.All()),
    owner_creatures(creatures.Owners())
    {
    }

    void Update() override {
        for (size_t i = 0; i < dense_creatures.size(); i++) {
            Creature &creature = dense_creatures[i];

            creature.Health += creature.HealthDelta;
            if (creature.Health <= 0) {
                world->DestroyEntity(owner_creatures[i]);
            }

            creature.HealthDelta = 0;
        }
    }
};
