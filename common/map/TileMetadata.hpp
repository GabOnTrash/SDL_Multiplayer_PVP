#pragma once

#include "TileTypes.hpp"

struct TileMetadata
{
	TileType type = TileType::Empty;
    bool isSolid = false;
};