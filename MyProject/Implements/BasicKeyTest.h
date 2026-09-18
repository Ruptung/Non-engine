#pragma once

#include "../Headers/IEventListener.h"

struct KeyFlags {
public:
    KeyFlags() : axis(Vector2::zero()) {}

    Vector2 axis;
};


class BasicKeyTest : public IEventListener {
public:
    BasicKeyTest(KeyFlags &keys) : keys(keys) {
    }

    void OnEvent(const SDL_Event &event) override {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.key) {
                case SDLK_UP:
                    keys.axis.y = 1;
                    break;
                case SDLK_DOWN:
                    keys.axis.y = -1;
                    break;
                case SDLK_LEFT:
                    keys.axis.x = -1;
                    break;
                case SDLK_RIGHT:
                    keys.axis.x = 1;
                    break;
            }
        }
        if (event.type == SDL_EVENT_KEY_UP) {
            switch (event.key.key) {
                case SDLK_UP:
                    keys.axis.y = 0;
                    break;
                case SDLK_DOWN:
                    keys.axis.y = 0;
                    break;
                case SDLK_LEFT:
                    keys.axis.x = 0;
                    break;
                case SDLK_RIGHT:
                    keys.axis.x = 0;
                    break;
            }
        }
    }

private:
    KeyFlags &keys;
};
