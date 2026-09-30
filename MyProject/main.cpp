#include <SDL3/SDL.h>
#include <vector>

#include "Headers/BasicStructures.h"
#include "Headers/IRender.h"
#include "Headers/IEventListener.h"
#include "Headers/IRule.h"
#include "Headers/Object.h"
#include "Headers/Painter.h"
#include "Headers/SDL_Wizard.h"
#include "Headers/Storage.h"

#include "Implements/EventListeners/BasicKeyTest.h"
#include "Implements/Renders/MapRenderer.h"
#include "Implements/Rules/MoveRule.h"
#include "Implements/Renders/TileRender.h"

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

    Vector2 WorldSize = Vector2(5, 5);
    uint16_t WorldMap[5][5] = {
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, 0, 0, 0, SampleTile},
        {SampleTile, 0, 0, 0, 0},
        {SampleTile, 0, 0, 0, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile}
    };
    uint16_t WorldSpec[5][5] = {
        {WorldFlag::FLAG_SOLID, WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID,  WorldFlag::FLAG_SOLID},
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

    //properties

    Storage<Transform> transforms;
    Storage<Renderable> renderables;
    Storage<Physic> physics;

    //entities
    Entity camera = 0;
    transforms.Add(camera , Transform(Vector2(3, 3)));

    Entity player = 1;
    transforms.Add(player, Transform(Vector2(1, 1)));
    renderables.Add(player, Renderable(&transforms.Get(player), TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0) | TILE_Y_IVRT));

    //rules
    BasicKeyTest basic_key_test = BasicKeyTest(keys);
    listeners.push_back(&basic_key_test);


    MoveRule my_rule = MoveRule(transforms.Get(player), keys, WorldSize, &WorldSpec[0][0]);
    rules.push_back(&my_rule);

    TileRender tile_render = TileRender(renderables.All());
    renders.push_back(&tile_render);

    MapRenderer map_renderer = MapRenderer(&WorldMap[0][0], WorldSize, VirtualVector, transforms.Get(camera).position);
    renders.push_back(&map_renderer);


    VirtualScreen vs(VirtualVector, transforms.Get(camera), 4, &tiles[0][0][0]);


    //loop
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
        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));

        SDL_Delay(41); //16ms = 60fps, 41ms = 24fps
    }
    return 0;
}
