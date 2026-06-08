#pragma once

#include <cstdint>

enum class PVP_PACKET : uint32_t
{
	MapSeed = 0,
    PlayerJoin,
    PlayerLeave,
    PlayerMove
};