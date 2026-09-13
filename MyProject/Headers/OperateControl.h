#pragma once

#include <cstdio>

#include "IOperator.h"

enum class keyInstruction : SDL_Keycode {
    Quit = SDLK_Q,
    Pressed_A = SDLK_A
};


class OperateControl : public IOperator {
public:
    OperateControl(SDL_Event &event) : event(event) {
    }

    void Update() override {
        switch ((keyInstruction) event.key.key) {
            case keyInstruction::Pressed_A:
                printf("A!");
                break;
            case keyInstruction::Quit:
                break;
        }
    }

private:
    SDL_Event &event;
};
