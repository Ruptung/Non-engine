#pragma once

#include "Headers/Interfaces/IScene.h"

#include "Headers/Addons/Tile_Addon/TileMapRenderer.h"
#include "Headers/Addons/Tile_Addon/TileObjectRender.h"
#include "Implements/Colliders/HurtOnCollide.h"

#include "Implements/EventListeners/BasicKeyTest.h"
#include "Implements/PersonalScript/HordeBehavior.h"

#include "Implements/Rules/ColliderRule.h"
#include "Implements/Rules/CreatureRule.h"
#include "Implements/Rules/PhysicsRule.h"

class SampleScene : public IScene {
private:
    const uint16_t SampleTile = TILE_ID(2) | TILE_PALETTE(0) | TILE_LAYER(1);

    uint16_t WorldMap[5][5] = {
        {0, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, 0, 0, 0, SampleTile},
        {SampleTile, 0, 0, 0, 0},
        {SampleTile, 0, 0, 0, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile}
    };
    uint16_t WorldSpec[5][5] = {
        {WorldFlag::FLAG_NONE, WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID},
        {WorldFlag::FLAG_SOLID, WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_SOLID},
        {WorldFlag::FLAG_SOLID, WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE},
        {WorldFlag::FLAG_SOLID, WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_NONE,   WorldFlag::FLAG_SOLID},
        {WorldFlag::FLAG_SOLID, WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID},
    };
public:
    SampleScene(ScreenSet &screen_set, KeyFlags &keys, TileSet &tile_set)
    : IScene(screen_set, keys)
    {
        //init
        WorldSet world_set = {
            Vector2Int(5, 5),
            &WorldMap[0][0],
            &WorldSpec[0][0]
        };

        //entities
        Entity camera = world.CreateEntity();
        world.AddComponent(camera , Transform(Vector2(-1, -1)));
        Camera = camera;

        Entity player = world.CreateEntity();
        world.AddComponent(player, Transform(Vector2(0, 0), Vector2(3, 3)));
        world.AddComponent(player, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0) | TILE_Y_IVRT));
        world.AddComponent(player, Collider());
        world.AddComponent(player, Physics());
        world.AddComponent(player, PlayerMoveScript(player, world.GetStorage<Transform>(), world.GetStorage<Physics>(), &keys, world_set, tile_set));

        Entity Manager = world.CreateEntity();
        world.AddComponent(Manager, HordeBehavior(player, world.GetStorage<Transform>(), world.GetStorage<Physics>(), world.GetStorage<Zombie>()));

        world.AddComponent(player, HurtOnCollide(world.GetStorage<Zombie>()));

        //Painter
        tile_painter.emplace(tile_set, *world.GetStorage<Transform>(), camera);

        //Push Interface
        auto &collider_rule =  AddRule<ColliderRule>(*world.GetStorage<Collider>(), *world.GetStorage<Transform>(), world);
        AddRule<PhysicsRule>(*world.GetStorage<Physics>(), *world.GetStorage<Transform>(), collider_rule, world_set, tile_set);
        AddRule<CreatureRule>(&world, *world.GetStorage<Creature>());

        AddRender<TileObjectRender>(*tile_painter, *world.GetStorage<Renderable>(), *world.GetStorage<Transform>());
        AddRender<TileMapRenderer>(*tile_painter, screen_set, tile_set, world_set, *world.GetStorage<Transform>(), camera);
    }
};
