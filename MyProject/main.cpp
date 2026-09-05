#include <cstdio>
#include <SDL3/SDL.h>

#include "Headers/SDL_Wizard.h"
#include "Headers/VirtualScreen.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024

int main() {
    SDL_Wizard wiz = SDL_Wizard(SCREEN_HEIGHT, SCREEN_WIDTH);

    bool running = true;

    SDL_Event event;

    VirtualScreen vs = VirtualScreen(32, 32);

    uint8_t tiles[][4][4] = {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        (0,1,0,1)
    };

    vs.SetTiles(&tiles[0][0][0], 4);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_Q:
                        vs.DrawTileOnGrid(1, 1, 0);
                        break;
                }
            }
        }

        SDL_Delay(16);
    }
    return 0;
}
