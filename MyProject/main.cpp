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
#include "Headers/Addons/Tile_Addon/TileSystem.h"
#include "Headers/World.h"

#include "Implements/Colliders/HurtOnCollide.h"
#include "Implements/Colliders/PrintOnCollide.h"

#include "Implements/EventListeners/BasicKeyTest.h"
#include "Headers/Addons/Tile_Addon/TileMapRenderer.h"
#include "Implements/PersonalScript/PlayerMoveScript.h"
#include "Headers/Addons/Tile_Addon/TileObjectRender.h"
#include "Implements/Rules/ColliderRule.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define VIRTUAL_WIDTH 36
#define VIRTUAL_HEIGHT 36


int main() {
    bool running = true;

    ScreenSet screen_set = {
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        {VIRTUAL_WIDTH, VIRTUAL_HEIGHT}
    };

    //datas
    uint32_t colors[][8] = {
        0x00000000, 0xFFFFFFFF, 0xFF777777, 0xFF0000FF
    };

    uint16_t SampleTile = TILE_ID(2) | TILE_PALETTE(0) | TILE_LAYER(1);

    //--------TODO Change this to external Data----------
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
    //---------------------------------------------------

    TileSet tile_set = {
        Vector2(4, 4),
        &tiles[0][0][0]
    };
    WorldSet world_set = {
        Vector2(5, 5),
        &WorldMap[0][0],
        &WorldSpec[0][0]
    };

    //init
    SDL_Wizard wiz(screen_set);
    Painter painter(screen_set.VirtualVector, &colors[0][0]);
    SDL_Event event;
    KeyFlags keys;

    //systems
    std::vector<IEventListener *> listeners;
    std::vector<IRender *> renders;
    std::vector<IRule *> rules;

    //properties  
    World world;

    //entities
    Entity camera = world.CreateEntity();
    world.AddComponent(camera , Transform(Vector2(-1, -1)));

    Entity player = world.CreateEntity();
    world.AddComponent(player, Transform(Vector2(0, 0), Vector2(4, 4)));
    world.AddComponent(player, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0) | TILE_Y_IVRT));
    world.AddComponent(player, Collider());
    world.AddComponent(player, PlayerMoveScript(player, &world.GetStorage<Transform>(), &keys, world_set, tile_set));


    Entity Dummy = world.CreateEntity();
    world.AddComponent(Dummy, Transform(Vector2(3, 3), Vector2(4, 4)));
    world.AddComponent(Dummy, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0)));
    world.AddComponent(Dummy, Collider());

    //world.AddComponent(player, PrintOnCollide());
    //world.AddComponent(Dummy, HurtOnCollide());

    TilePainter tile_painter(tile_set, world.GetComponent<Transform>(camera));

    //Interface
    BasicKeyTest basic_key_test = BasicKeyTest(keys);
    ColliderRule collider_rule = ColliderRule(world.GetStorage<Collider>(), world.GetStorage<Transform>(), world);

    TileObjectRender object_render = TileObjectRender(tile_painter, world.GetStorage<Renderable>(), world.GetStorage<Transform>());
    TileMapRenderer map_renderer = TileMapRenderer(tile_painter, screen_set, tile_set, world_set, world.GetStorage<Transform>(), camera);

    //Push Interface
    listeners.push_back(&basic_key_test);

    rules.push_back(&collider_rule);

    renders.push_back(&object_render);
    renders.push_back(&map_renderer);

    VirtualScreen virtual_screen(screen_set);

    for (auto &view : world.Implementers<IScript>())
        for (size_t i = 0; i < view->Size(); i++)
            view->At(i).Start();

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

        virtual_screen.Clear();
        for (IRender *render: renders)
            render->Render(virtual_screen);

        for (auto &view : world.Implementers<IScript>())
            for (size_t i = 0; i < view->Size(); i++)
                view->At(i).Update();

        wiz.OverwriteBuffer(painter.GetScreen(virtual_screen.GetScreen()));
        SDL_Delay(41); //16ms = 60fps, 41ms = 24fps
    }
    return 0;
}
