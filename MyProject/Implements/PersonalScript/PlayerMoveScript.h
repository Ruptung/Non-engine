#pragma once

#include "Headers/Interfaces/IScript.h"
#include "Headers/World.h"
#include "Headers/Interfaces/IRule.h"
#include "Headers/Storage.h"
#include "Headers/Object.h"
#include "../../Headers/Addons/Tile_Addon/TileSystem.h"

#include "Implements/EventListeners/BasicKeyTest.h"

enum WorldFlag : uint16_t {
    FLAG_NONE       = 0,
    FLAG_SOLID      = 1 << 0,
    // WIP ...
};

class PlayerMoveScript : public IScript {
private:
    Storage<Transform> *transforms;
    Storage<Physics> *physics;
    Vector2 WorldSize;
    uint16_t *WorldSpec;
    Vector2 TileSize;
    KeyFlags *keys;
public:
    PlayerMoveScript(Entity player, Storage<Transform> *transforms, Storage<Physics> *physics, KeyFlags *keys, const WorldSet &WorldSet, const TileSet TileSet)
    : transforms(transforms),
    physics(physics),
    WorldSize(WorldSet.WorldSize),
    WorldSpec(WorldSet.WorldSpec),
    TileSize(TileSet.TileSize),
    keys(keys)
    {
        thisEntity = player;
    }


    void Start() override {

    }

    void Update() override {
        physics->Get(thisEntity)->Velocity = keys->axis;
    }
private:
};
