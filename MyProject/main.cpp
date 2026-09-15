#include <SDL3/SDL.h>
#include <vector>

#include "Headers/BasicStructures.h"
#include "Headers/IRender.h"
#include "Headers/IEventListener.h"
#include "Headers/Painter.h"
#include "Headers/SDL_Wizard.h"

#include "Implements/BasicKeyTest.h"
#include "Implements/TileRender.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define VIRTUAL_WIDTH 32
#define VIRTUAL_HEIGHT 32


int main() {
    Vector2 ScreenVector = { SCREEN_WIDTH, SCREEN_HEIGHT };
    Vector2 VirtualVector = { VIRTUAL_WIDTH, VIRTUAL_HEIGHT };

    bool running = true;



    uint32_t colors[][8] = {
        0xFF000000, 0xFFFFFFFF, 0xFF00FF00, 0xFF0000FF
    };
    uint8_t tiles[][4][4] = {
        {
            {2,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,1,0,1}
        }
    };

    SDL_Wizard wiz(VirtualVector, ScreenVector);
    VirtualScreen vs(VirtualVector, 4, &tiles[0][0][0]);
    Painter painter(VirtualVector, &colors[0][0]);
    SDL_Event event;

    std::vector<IEventListener*> listeners;
    std::vector<IRender*> renders;

    BasicKeyTest basic_key_test = BasicKeyTest();
    listeners.push_back(&basic_key_test);

    TileRender tile_render = TileRender();
    renders.push_back(&tile_render);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            for (IEventListener* listener : listeners)
                listener->OnEvent(event);
        }

        for (IRender* render : renders)
            render->Render(vs);
        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));

        SDL_Delay(16);
    }
    return 0;
}
