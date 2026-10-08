#include <SDL3/SDL.h>
#include <vector>

#include "Headers/BasicStructures.h"

#include "Headers/Interfaces/IRender.h"
#include "Headers/Interfaces/IEventListener.h"
#include "Headers/Interfaces/IRule.h"
#include "Headers/Interfaces/IScript.h"

#include "Headers/Object.h"
#include "Headers/Painter.h"
#include "Headers/SDL_Wizard.h"
#include "Headers/Storage.h"
#include "Headers/World.h"

#include "Implements/HurtOnCollide.h"
#include "Implements/PrintOnCollide.h"

#include "Implements/EventListeners/BasicKeyTest.h"
#include "Implements/Renders/MapRenderer.h"
#include "Implements/Rules/MoveRule.h"
#include "Implements/Renders/ObjectRender.h"
#include "Implements/Rules/ColliderRule.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define VIRTUAL_WIDTH 36
#define VIRTUAL_HEIGHT 36


int main() {
    bool running = true;

    //datas
    Vector2 ScreenVector = {SCREEN_WIDTH, SCREEN_HEIGHT};
    Vector2 VirtualVector = {VIRTUAL_WIDTH, VIRTUAL_HEIGHT};

    uint32_t colors[][8] = {
        0x00000000, 0xFFFFFFFF, 0xFF777777, 0xFF0000FF
    };

    //TODO Make TileData Structure which compress all the tile data.
    Vector2 tileSize = Vector2(4, 4 );
    uint8_t tiles[][4][4] = {
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {1, 0, 0, 1},
            {0, 0, 0, 0},
            {1, 0, 0, 1},
            {0, 1, 1, 0}
        },
        {
            {2, 2, 0, 0},
            {2, 2, 0, 0},
            {0, 0, 2, 2},
            {0, 0, 2, 2}
        }
    };

    uint16_t SampleTile = TILE_ID(2) | TILE_PALETTE(0) | TILE_LAYER(1);

    Vector2 WorldSize = Vector2(20, 20);
    uint16_t WorldMap[20][20] = {
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

    //init
    SDL_Wizard wiz(VirtualVector, ScreenVector);
    Painter painter(VirtualVector, &colors[0][0]);
    SDL_Event event;
    KeyFlags keys;

    //systems
    std::vector<IEventListener *> listeners;
    std::vector<IRender *> renders;
    std::vector<IRule *> rules;
    std::vector<IScript *> scripts;

    //properties
    World world;

    //entities
    Entity camera = world.CreateEntity();
    world.AddComponent(camera , Transform(Vector2(3, 3)));

    Entity player = world.CreateEntity();
    world.AddComponent(player, Transform(Vector2(0, 0), Vector2(4, 4)));
    world.AddComponent(player, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0) | TILE_Y_IVRT));
    world.AddComponent(player, Collider());



    Entity Dummy = world.CreateEntity();
    world.AddComponent(Dummy, Transform(Vector2(3, 3), Vector2(4, 4)));
    world.AddComponent(Dummy, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0)));
    world.AddComponent(Dummy, Collider());

    world.AddComponent(player, PrintOnCollide());
    world.AddComponent(Dummy, HurtOnCollide());

    //Interface
    BasicKeyTest basic_key_test = BasicKeyTest(keys);

    MoveRule my_rule = MoveRule(world.GetStorage<Transform>(), player, keys, WorldSize, &WorldSpec[0][0]);
    ColliderRule collider_rule = ColliderRule(world.GetStorage<Collider>(), world.GetStorage<Transform>(), world);

    ObjectRender tile_render = ObjectRender(world.GetStorage<Renderable>(), world.GetStorage<Transform>());
    MapRenderer map_renderer = MapRenderer(&WorldMap[0][0], WorldSize, VirtualVector, tileSize, world.GetStorage<Transform>(), camera);


    //Push Interface
    listeners.push_back(&basic_key_test);

    rules.push_back(&my_rule);
    rules.push_back(&collider_rule);

    renders.push_back(&tile_render);
    renders.push_back(&map_renderer);

    VirtualScreen vs(VirtualVector, world.GetComponent<Transform>(camera), 4, &tiles[0][0][0]);


    for (IScript *script: scripts)
        script->Start();

    // //loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            for (IEventListener *listener: listeners)
                listener->OnEvent(event);

            if (event.type == SDL_EVENT_QUIT || keys.exit)
                running = false;
        }

        for (IRule *rule: rules)
            rule->Update();

        vs.Clear();
        for (IRender *render: renders)
            render->Render(vs);

        for (IScript *script: scripts)
            script->Update();

        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));
        SDL_Delay(41); //16ms = 60fps, 41ms = 24fps
    }
    return 0;
}
