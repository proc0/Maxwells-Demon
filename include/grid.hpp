#pragma once

#include <array>
#include <vector>
#include <algorithm> // IWYU pragma: keep
#include "common.hpp" // IWYU pragma: keep

#define MAX_CACHE_SIZE 9

using Cell = std::vector<short>;

class Grid {
    std::vector<std::vector<Cell>> cells;
    Vector2 dimension = { 10, 10 };
    std::array<short, MAX_CACHE_SIZE+1> cache;
    float unit = 1;
    short cursor = 0;

public:
    Grid(){};
    ~Grid() = default;

    void Load(short width, short height, float unit);
    Vector2 GridPoint(float x, float y) const;
    void AddPoint(Vector2 point, short id);
    void RemovePoint(Vector2 cell, short id);
    Vector2 UpdatePoint(Vector2 point, Vector2 cell, short id);
    Cell ZonePoint(Vector2 point, float radius, short id);

    void AddArea(Rectangle rect, short id);
    void RemoveArea(Rectangle area, short id);
    Cell ZoneArea(Vector2 point, float radius) const;

    void CacheClear();
};
