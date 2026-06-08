#pragma once

#include <vector>
#include "TileMetadata.hpp"

class AdvancedMap
{
public:
    int rows = 0;
	int columns = 0;
	std::vector<std::vector<TileMetadata>> grid;
	
	AdvancedMap(int width, int height)
		: rows(height), columns(width)
	{
		grid.resize(height, std::vector<TileMetadata>(width));
	}
};