#pragma once

#include <vector>
#include <raylib.h>
#include <raymath.h>
#include <algorithm> // IWYU pragma: keep

#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 240
#define CONTAINER_Y 200
#define CONTAINER_BORDER 15
#define CONTAINER_BORDER_WIDTH CONTAINER_WIDTH + CONTAINER_BORDER*2
#define CONTAINER_BORDER_HEIGHT CONTAINER_HEIGHT + CONTAINER_BORDER*2
#define CONTAINER_BORDER_X CONTAINER_X - CONTAINER_BORDER
#define CONTAINER_BORDER_Y CONTAINER_Y - CONTAINER_BORDER

using Cell = std::vector<short>;

namespace Locate {
    // float Top(const Molecule&);
    float Top(Vector2 position, float radius);
    // float Left(const Molecule&);
    float Left(Vector2 position, float radius);
    // float Right(const Molecule&);
    float Right(Vector2 position, float radius);
    // float Bottom(const Molecule&);
    float Bottom(Vector2 position, float radius);
};

class Grid
{
    std::vector<std::vector<Cell>> cells;
    // std::vector<std::vector<Cell>> wallCells;
    // Rectangle sensor = Rectangle({610, 300, 60, 200});
    Vector2 cellCount;
    float cellSize;
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
