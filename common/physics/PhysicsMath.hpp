#pragma once

#include <cmath>

struct vec2
{
	float x;
	float y;

	vec2 operator+(const vec2& other) const
	{
		return {x + other.x, y + other.y};
	}
	vec2 operator-(const vec2& other) const
	{
		return {x - other.x, y - other.y};
	}
	vec2 operator*(float scalar) const
	{
		return {x * scalar, y * scalar};
	}
	vec2 operator/(float scalar) const
	{
		return {x / scalar, y / scalar};
	}
    float operator[](int index) const
	{
		return index == 0 ? x : y;
    }

	float distanceTo(const vec2& other) const
	{
		float dx = x - other.x;
		float dy = y - other.y;
		return sqrt(dx * dx + dy * dy);
    }
};

struct Box
{
	vec2 pos; 
	vec2 size;    

	bool Intersects(const Box& other) const
	{
		return (pos.x < other.pos.x + other.size.x &&
				pos.x + size.x > other.pos.x &&
				pos.y < other.pos.y + other.size.y &&
				pos.y + size.y > other.pos.y);
    }
};