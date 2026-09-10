#include <cstdio>
#include <SDL3/SDL.h>

#include "Headers/SDL_Wizard.h"
#include "Headers/VirtualScreen.h"
#include "Headers/Painter.h"

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32
#define VIRTUAL_WIDTH 32
#define VIRTUAL_HEIGHT 32

int main() {
    SDL_Wizard wiz = SDL_Wizard(SCREEN_HEIGHT, SCREEN_WIDTH);

    bool running = true;

    SDL_Event event;

    VirtualScreen vs = VirtualScreen(VIRTUAL_HEIGHT, VIRTUAL_WIDTH);
    Painter painter = Painter(SCREEN_HEIGHT, SCREEN_WIDTH, VIRTUAL_HEIGHT, VIRTUAL_WIDTH);

    uint32_t colors[] = { 0xFF000000, 0xFFFFFFFF, 0xFF00FF00, 0xFF0000FF};
    painter.SetPalette(colors);

    uint8_t tiles[][4][4] = {
        {
            {1,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,1,0,1}
        }
    };

    vs.SetTiles(&tiles[0][0][0], 4);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_Q:

                        break;
                }
            }
        }


        vs.DrawTileOnGrid(0, 0, TILE_ID(0) | TILE_LAYER(1) | TILE_PALETTE(0));

        wiz.OverwriteBuffer(painter.GetScreen(vs.GetScreen()));
        SDL_Delay(16);
    }
    return 0;
}
