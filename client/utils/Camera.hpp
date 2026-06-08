#pragma once

#include "../../common/physics/PhysicsMath.hpp"

struct Camera
{
    vec2 position{0, 0};
    vec2 size{0, 0};

    vec2 world{3840,2160};

    void Follow(const Box& target)
    {
        this->position = target.pos + target.size * 0.5f - size * 0.5f;

        /*if (position.x < 0)
            position.x = 0;
        if (position.y < 0)
            position.y = 0;
        if (position.x + size.x > world.x)
            position.x = 3840 - size.x;
        if (position.y + size.y > world.y)
            position.y = 2160 - size.y;*/
    }
};