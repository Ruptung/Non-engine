#pragma once

class IOperator {
public:
    virtual ~IOperator() = default;
    virtual void Update() = 0;
};