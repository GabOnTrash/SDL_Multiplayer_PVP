#pragma once

#include "../../common/physics/PhysicsMath.hpp"

struct Camera
{
    vec2 position{0, 0};
    vec2 size{0, 0};

    void Follow(const Box& target)
    {
        this->position = target.pos + target.size * 0.5f - size * 0.5f;
    }
};