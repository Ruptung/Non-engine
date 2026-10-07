#pragma once
#include <cstdio>

#include "Headers/IScript.h"

class PersonalScriptTest : public IScript {
public:
    void Start() override {
        printf("Start\n");
    }

    void Update() override {
        //printf("Update\n");
    }
private:
};
