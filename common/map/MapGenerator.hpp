#pragma once

#include <cmath>
#include <random>
#include <vector>
#include "AdvancedMap.hpp"
#include "../physics/PhysicsEngine.hpp"

class MapGenerator
{
public:
    MapGenerator()
    {
        std::random_device rd;
        m_rng = std::mt19937(rd());
    }

    void SetSeed(uint32_t seed)
    {
        m_rng.seed(seed);
    }

    AdvancedMap Generate(bool isServer)
    {
        AdvancedMap map(MAP_WIDTH_IN_TILES, MAP_HEIGHT_IN_TILES);
        int tilesPlaced = 0;

        int floorRows[TOTAL_FLOORS];
        // start from the bottom
        int baseFloor = MAP_HEIGHT_IN_TILES - 4;

        for (int i = 0; i < TOTAL_FLOORS; ++i)
            floorRows[i] = baseFloor - (i * (FLOORS_SPACING + 1));

        std::vector<int> platformSizes = {3, 4, 5, 6};
        std::uniform_int_distribution<int> sizeDist(0, platformSizes.size() - 1);
        std::uniform_int_distribution<int> floorDist(0, TOTAL_FLOORS - 1);
        std::uniform_int_distribution<int> columnDist(2, MAP_WIDTH_IN_TILES - 8);

        int attempts = 0;

        while (tilesPlaced < MAX_TILES_TO_GENERATE && attempts < 500)
        {
            attempts++;

            int targetFloor = floorDist(m_rng);
            int startCol = columnDist(m_rng);
            int length = platformSizes[sizeDist(m_rng)];
            int selectedRow = floorRows[targetFloor];

            if (tilesPlaced + length >= MAX_TILES_TO_GENERATE)
                length = MAX_TILES_TO_GENERATE - tilesPlaced;

            if (length <= 0)
                break;

            bool spaceAvailable = true;

            for (int c = startCol - 2; c < startCol + length + 2; c++)
            {
                if (c >= 0 && c < MAP_WIDTH_IN_TILES)
                {
                    if (map.grid[selectedRow][c].type != TileType::Empty)
                    {
                        spaceAvailable = false;
                        break;
                    }
                }
            }

            if (spaceAvailable)
            {
                for (int c = startCol; c < startCol + length; c++)
                {
                    map.grid[selectedRow][c].type = TileType::Solid;
                    map.grid[selectedRow][c].isSolid = true;
                }
                tilesPlaced+= length;
            }
        }

        if (!isServer)
            AutoTileMap(map);

        return map;
    }
    
    void AddPhysicsToMap(const AdvancedMap& map, PhysicsEngine& physicsEngine)
    {
        const float TILE_SIZE = 64.0f;

        for (int r = 0; r < map.rows; r++)
        {
            int startCol = -1;
            int length = 0;

            for (int c = 0; c < map.columns; c++)
            {
                if (map.grid[r][c].isSolid)
                {
                    if (startCol == -1)
                        startCol = c; // start of a new platform
                    length++;
                }
                else
                {
                    if (startCol != -1)
                    {
                        PhysicsBody* body = new PhysicsBody();

                        body->bounds.pos = {startCol * TILE_SIZE, r * TILE_SIZE};
                        body->bounds.size = {length * TILE_SIZE, TILE_SIZE};

                        body->isStatic = true;

                        physicsEngine.AddBody(body);

                        startCol = -1; // reset for the next platform
                        length = 0;
                    }
                }
            }

            if (startCol != -1)
            {
                PhysicsBody* body = new PhysicsBody();

                body->bounds.pos = {startCol * TILE_SIZE, r * TILE_SIZE};
                body->bounds.size = {length * TILE_SIZE, TILE_SIZE};
                body->isStatic = true;

                physicsEngine.AddBody(body);
            }
        }
    }

private:
    void AutoTileMap(AdvancedMap& map)
    {
        for (int r = 0; r < map.rows; ++r)
        {
            for (int c = 0; c < map.columns; ++c)
            {
                if (map.grid[r][c].type != TileType::Solid)
                    continue;
                
                bool upEmpty = (r == 0) || (map.grid[r - 1][c].type == TileType::Empty);
                bool downEmpty = (r == map.rows - 1) || (map.grid[r + 1][c].type == TileType::Empty);
                bool leftEmpty = (c == 0) || (map.grid[r][c - 1].type == TileType::Empty);
                bool rightEmpty = (c == map.columns - 1) || (map.grid[r][c + 1].type == TileType::Empty);

                if (upEmpty && leftEmpty)
                    map.grid[r][c].type = TileType::TopLeft;

                else if (upEmpty && rightEmpty)
                    map.grid[r][c].type = TileType::TopRight;
              
                else if (upEmpty)
                    map.grid[r][c].type = TileType::Top;

                else if (leftEmpty && downEmpty)
                    map.grid[r][c].type = TileType::BottomLeft;
               
                else if (rightEmpty && downEmpty)
                    map.grid[r][c].type = TileType::BottomRight;
                
                else if (downEmpty)
                    map.grid[r][c].type = TileType::Bottom;

                map.grid[r][c].isSolid = true;
            }
        }
    }

    const int MAP_WIDTH_IN_TILES = 60; // 4k / 64
    const int MAP_HEIGHT_IN_TILES = 34; // 2160p / 64
    const int MAX_TILES_TO_GENERATE = 50;
    const int FLOORS_SPACING = 3;
    const int TOTAL_FLOORS = 4;

    std::mt19937 m_rng;
};