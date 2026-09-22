#include <SDL3/SDL.h>
#include <vector>

#include "Headers/BasicStructures.h"
#include "Headers/IRender.h"
#include "Headers/IEventListener.h"
#include "Headers/IRule.h"
#include "Headers/Painter.h"
#include "Headers/SDL_Wizard.h"

#include "Implements/BasicKeyTest.h"
#include "Implements/MapRenderer.h"
#include "Implements/MyRule.h"
#include "Implements/TileRender.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define VIRTUAL_WIDTH 32
#define VIRTUAL_HEIGHT 32


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

    uint16_t map[5][5] = {
        {0, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile},
        {SampleTile, SampleTile, SampleTile, SampleTile, SampleTile}
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
    std::vector<Transform *> transforms;
    std::vector<Renderable *> Renderables;
    std::vector<Physic *> physics;

    //entities
    Transform camera = Transform(Vector2(3, 3));
    transforms.push_back(&camera);
    Renderables.push_back(nullptr);

    Transform player = Transform(Vector2(0, 0));
    transforms.push_back(&player);
    Renderable renderable = Renderable(&player, TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(0) | TILE_Y_IVRT);
    Renderables.push_back(&renderable);

    //rules
    BasicKeyTest basic_key_test = BasicKeyTest(keys);
    listeners.push_back(&basic_key_test);


    MyRule my_rule = MyRule(keys, transforms);
    rules.push_back(&my_rule);


    TileRender tile_render = TileRender(Renderables);
    renders.push_back(&tile_render);

    MapRenderer map_renderer = MapRenderer(&map[0][0], WorldSize, VirtualVector, player.position);
    renders.push_back(&map_renderer);


    VirtualScreen vs(VirtualVector, camera, 4, &tiles[0][0][0]);


    //loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            for (IEventListener *listener: listeners)
                listener->OnEvent(event);
        }

        for (IRule *rule: rules)
            rule->Update();

        vs.Clear();
        for (IRender *render: renders)
            render->Render(vs);
        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));

        SDL_Delay(41); //16 = 60fps, 41 = 24fps
    }
    return 0;
}
