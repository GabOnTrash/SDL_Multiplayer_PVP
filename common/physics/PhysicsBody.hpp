#pragma once

#include "PhysicsMath.hpp"

struct PhysicsBody
{
    Box bounds;
    vec2 velocity{0.0f, 0.0f};

	bool isOneWay = false; // for platforms that can be jumped through from below
	bool isStatic = false;
    bool isGrounded = false;

    float inputX = 0.0f; // -1 for left, 1 for right, 0 for no input
	float maxSpeed = 500.0f;
    float groundAccel = 999999.0f;
    float airAccel = 500.0f;
    float groundFriction = 999999.0f;

	void Resize(const vec2& newSize)
	{
		bounds.size = newSize;
    }
};