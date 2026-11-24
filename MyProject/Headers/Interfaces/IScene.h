#pragma once

#include "Implements/EventListeners/BasicKeyTest.h"

#include "Headers/World.h"

#include "IRender.h"
#include "IRule.h"

class IScene {

public:
    World world;
    ScreenSet &screen_set;

    std::optional<TilePainter> tile_painter;

    std::vector<std::unique_ptr<IRender>>   renders;
    std::vector<std::unique_ptr<IRule>>     rules;

    KeyFlags &keys;

    Entity Camera{};

    virtual ~IScene() = default;

    IScene(ScreenSet &screen_set, KeyFlags &keys)
    : screen_set(screen_set),
    keys(keys)
    {}

protected:
    template<typename T, typename... Args>
    T &AddRule(Args&&... args) {
        auto p = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *p;
        rules.push_back(std::move(p));
        return ref;
    }
    template<typename T, typename... Args>
    T &AddRender(Args&&... args) {
        auto p = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *p;
        renders.push_back(std::move(p));
        return ref;
    }
};
