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
    Vector2 WorldSize;
    uint16_t *WorldSpec;
    Vector2 TileSize;
    KeyFlags *keys;
public:
    PlayerMoveScript(Entity player, Storage<Transform> *transforms, KeyFlags *keys, WorldSet WorldSet, TileSet TileSet)
    : transforms(transforms),
    WorldSize(WorldSet.WorldSize),
    WorldSpec(WorldSet.WorldSpec),
    TileSize(TileSet.TileSize),
    keys(keys) {
        thisEntity = player;
    }


    void Start() override {

    }

    void Update() override {
        Transform &player = transforms->Get(thisEntity);

        if (!CanMove(player.position, keys->axis, player.scale))
            return;


        player.position = player.position + keys->axis;
    }
private:
    bool CanMove(Vector2 pos, Vector2 moveto, Vector2 scale) {
        if (scale.x * scale.y == 0) { return true; }
        else { scale = scale - Vector2::one(); }

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
