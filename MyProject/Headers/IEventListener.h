#pragma once

#include <SDL3/SDL_events.h>

class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void OnEvent(const SDL_Event& event) = 0;
};
