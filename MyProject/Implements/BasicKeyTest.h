#pragma once

#include "../Headers/IEventListener.h"

enum class keyInstruction : SDL_Keycode {
    Quit = SDLK_Q,
    Pressed_A = SDLK_A
};


class BasicKeyTest : public IEventListener {
public:
    void OnEvent(const SDL_Event& event) override {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch ((keyInstruction)event.key.key) {
                case keyInstruction::Pressed_A:
                    printf("A!");
                    break;
                case keyInstruction::Quit:
                    break;
            }
        }
    }
};
