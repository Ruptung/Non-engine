#pragma once

#include <vector>

#include "../Headers/BasicStructures.h"

#include "../Headers/IRule.h"

class MyRule : public IRule {
public:
    MyRule(std::vector<Vector2> &vectors) : vectors(vectors) {};
    void Update() override {

    }
private:
    std::vector<Vector2> vectors;
};