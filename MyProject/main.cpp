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

#include "Implements/EventListeners/BasicKeyTest.h"
#include "Headers/Addons/Tile_Addon/TileMapRenderer.h"
#include "Headers/Addons/Tile_Addon/TileObjectRender.h"
#include "Implements/Scenes/SampleScene.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define VIRTUAL_WIDTH 36
#define VIRTUAL_HEIGHT 36

void Summon(World &world) {
    Entity z1_test = world.CreateEntity();
    world.AddComponent(z1_test, Transform(Vector2(5, 5), Vector2(3, 3)));
    world.AddComponent(z1_test, Renderable(TILE_ID(1) | TILE_LAYER(2) | TILE_PALETTE(1)));
    world.AddComponent(z1_test, Zombie());
    world.AddComponent(z1_test, Collider());
    world.AddComponent(z1_test, Physics());
    world.AddComponent(z1_test, Creature(20, 0));
}

int main() {
    bool running = true;

    ScreenSet screen_set = {
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        {VIRTUAL_WIDTH, VIRTUAL_HEIGHT}
    };

    //datas
    uint32_t colors[][8] = {
        {0x00000000, 0xFFFFFFFF, 0xFF777777, 0xFF0000FF,},
        {0x00000000, 0xFF00FF00}
    };

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
    //---------------------------------------------------

    TileSet tile_set = {
        Vector2Int(4, 4),
        &tiles[0][0][0]
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

    //Event Listeners
    BasicKeyTest basic_key_test = BasicKeyTest(keys);
    listeners.push_back(&basic_key_test);

    //init Scene
    std::unique_ptr<IScene> current_scene = std::make_unique<SampleScene>(screen_set, keys, tile_set);

    VirtualScreen virtual_screen(screen_set);

    for (auto &view : current_scene->world.Implementers<IScript>())
        for (size_t i = 0; i < view->Size(); i++)
            view->At(i).Start();

    // //loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            for (IEventListener *listener: listeners)
                listener->OnEvent(event);

            if (event.type == SDL_EVENT_QUIT || keys.exit) { running = false; }
            if (keys.interact) {
                current_scene = std::make_unique<SampleScene>(screen_set, keys, tile_set);
            }
        }

        for (auto &view : current_scene->world.Implementers<IScript>())
            for (size_t i = 0; i < view->Size(); i++)
                view->At(i).Update();

        for (auto &rule: current_scene->rules)
            rule->Update();

        current_scene->world.FlushDestroyed();

        virtual_screen.Clear();
        for (auto &render: current_scene->renders)
            render->Render(virtual_screen);

        wiz.OverwriteBuffer(painter.GetScreen(virtual_screen.GetScreen()));
        SDL_Delay(41); //16ms = 60fps, 41ms = 24fps
    }
    return 0;
}
