#pragma once

#include <cstdint>

enum class pvpPacket : uint32_t
{
	MapSeed = 0,
    PlayerJoin,
    PlayerLeave,
    PlayerMove
};