#include <SDL3/SDL.h>

#include "Headers/IOperator.h"
#include "Headers/OperateControl.h"
#include "Headers/OperateRender.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define VIRTUAL_WIDTH 32
#define VIRTUAL_HEIGHT 32


int main() {
    bool running = true;

    SDL_Event event;

    uint32_t colors[][8] = {
        0xFF000000, 0xFFFFFFFF, 0xFF00FF00, 0xFF0000FF
    };
    uint8_t tiles[][4][4] = {
        {
            {1,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,1,0,1}
        }
    };

    IOperator *currentPointer = nullptr;
    OperateControl operate_control = OperateControl(event);
    OperateRender operate_render = OperateRender(
        VIRTUAL_HEIGHT, VIRTUAL_WIDTH,
        SCREEN_HEIGHT, SCREEN_WIDTH,
        &colors[0][0], &tiles[0][0][0]
        );

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            currentPointer = &operate_control;
            currentPointer->Update();
        }

        currentPointer = &operate_render;
        currentPointer->Update();

        SDL_Delay(16);
    }
    return 0;
}
