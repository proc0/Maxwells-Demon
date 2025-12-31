#pragma once

#include <array>
#include <vector>
#include <algorithm> // IWYU pragma: keep
#include "common.hpp" // IWYU pragma: keep

using Cell = std::vector<short>;

class Grid
{
    std::vector<std::vector<Cell>> cells;
    std::array<short, 6> cache;
    // std::vector<std::vector<Cell>> wallCells;
    // Rectangle sensor = Rectangle({610, 300, 60, 200});
    Vector2 cellCount = { 10, 10 };
    float cellSize = 1;
    short cacheIndex = 0;
    // int doorFrame = 0;
    // bool isDoorClosing = false;

public:

    Grid(){};
    ~Grid() = default;

    void Load(short width, short height, float unit);
    Vector2 place(float x, float y) const;
    void addPoint(Vector2 point, short id);
    void remove(Vector2 cell, short id);
    Vector2 update(Vector2 point, Vector2 cell, short id);
    Cell getZone(Vector2 point, float radius, short id);

    void addArea(Rectangle rect, short id);
    void removeArea(Rectangle area, short id);
    Cell getArea(Vector2 point, float radius) const;

    // TODO: move to chamber class
    // bool checkSensor(Molecule &mol);
    // bool checkTunneling(Vector2 position, float radius);
    // void addWalls(std::vector<Rectangle> &wallRects);
    // void updateWalls();
    // void Render() const;
    // TODO: consolidate walls with one Grid class? 
    // void addWall(const Wall2& wall);
    // void removeWall(const Wall2& wall);
    // Cell getWalls(Molecule *mol);
};
