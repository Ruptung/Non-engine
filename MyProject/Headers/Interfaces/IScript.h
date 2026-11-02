#pragma once

#include "Headers/Storage.h"

class IScript {
public:
    virtual ~IScript() = default;

    virtual void Start() = 0;

    virtual void Update() = 0;
protected:
    Entity thisEntity = 0;
};
