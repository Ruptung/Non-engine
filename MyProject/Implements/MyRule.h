#pragma once

#include <vector>

#include "../Headers/BasicStructures.h"

#include "../Headers/IRule.h"
#include "BasicKeyTest.h"

class MyRule : public IRule {
public:
    MyRule(KeyFlags &keys ,std::vector<Transform*>& transforms) :
    transforms(transforms), keys(keys) {};
    void Update() override {
        transforms[1]->position = transforms[1]->position + keys.axis;
    }
private:
    std::vector<Transform*> &transforms;
    KeyFlags &keys;
};