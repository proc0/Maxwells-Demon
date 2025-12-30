#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <array>
#include <algorithm> // IWYU pragma: keep

#include "chamber.hpp"

#define DENSITY 40
#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 240
#define CONTAINER_Y 200
#define CONTAINER_BORDER 15
#define CONTAINER_BORDER_WIDTH CONTAINER_WIDTH + CONTAINER_BORDER*2
#define CONTAINER_BORDER_HEIGHT CONTAINER_HEIGHT + CONTAINER_BORDER*2
#define CONTAINER_BORDER_X CONTAINER_X - CONTAINER_BORDER
#define CONTAINER_BORDER_Y CONTAINER_Y - CONTAINER_BORDER
#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60


#define MAX_SPEED 500.0f
#define RESTITUTION 0.8f
#define MOLECULE_RADIUS 12.0f
#define CONTAINER_RIGHT (CONTAINER_X + CONTAINER_WIDTH - 3)
#define CONTAINER_BOTTOM (CONTAINER_Y + CONTAINER_HEIGHT - 3)
#define GRAVITY Vector2(0.0f, 982.0f)
#define ZERO_VECTOR Vector2(0.0f, 0.0f)

#define EASE_OUT_EXPO(x) ((x) >= 1 ? 1 : 1 - pow(2, -10 * (x)))

typedef struct Molecule
{
    Vector2 force{};
    Vector2 origin{};
    Vector2 position{};
    Vector2 velocity{};
    Vector2 acceleration{};
    Vector2 cell{};
    Color color1;
    Color color2;
    Color color;
    float mass = 0.0f;
    float radius = 0.0f;
    float restitution = RESTITUTION;
    short id = 0;
    bool active = false;
    bool collided = false;
    bool isHot = false;
    bool isLeft = false;
    bool isCounted = false;
    //TODO: hook this up to avoid processing molecule more than once in getZone
    bool isProcessed = false;
} Molecule;

namespace Locate {
    float Top(const Molecule&);
    float Top(Vector2 position, float radius);
    float Left(const Molecule&);
    float Left(Vector2 position, float radius);
    float Right(const Molecule&);
    float Right(Vector2 position, float radius);
    float Bottom(const Molecule&);
    float Bottom(Vector2 position, float radius);
};

// typedef struct Wall
// {
//     Rectangle rect;
//     std::vector<Vector2> cells;
//     int id;
// } Wall;

using Cell = std::vector<short>;

class Grid
{
    std::vector<std::vector<Cell>> cells;
    std::vector<std::vector<Cell>> wallCells;
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
    Cell getArea(Vector2 point, float radius);

    // TODO: move to chamber class
    // bool checkSensor(Molecule &mol);
    // bool checkTunneling(Vector2 position, float radius);
    // void addWalls(std::vector<Rectangle> &wallRects);
    // void updateWalls();
    // void Render() const;
    // TODO: consolidate walls with one Grid class? 
    void addWall(const Wall2& wall);
    void removeWall(const Wall2& wall);
    Cell getWalls(Molecule *mol);
};

class Gas
{
    Grid grid;
    std::array<Molecule, DENSITY> molecules;
    Rectangle containerLeft;
    Rectangle containerRight;
    std::vector<Rectangle> wallRects = {Rectangle({633, 200, 15, 150}), Rectangle({633, 350, 15, 100}), Rectangle({633, 450, 15, 150})};
    Color colorChamberLeft;
    Color colorChamberRight;
    Color barColor;

    const char *pipeText = ":";
    float rightChamberHotCount = 0;
    float rightChamberCoolCount = 0;
    float leftChamberHotCount = 0;
    float leftChamberCoolCount = 0;
    float totalHotCount = 0;
    float totalCoolCount = 0;
    float completion = 0.0f;
    float entropy = 0.0f;
    float maxEntropy = 0.0f;
    int entropyBarLength = CONTAINER_BORDER_WIDTH - 100;
    int entropyBar = 0;
    int entropyBarX = CONTAINER_BORDER_X + 50;
    int entropyBarY = 40;

public:
    int screenWidth = 1280;
    int screenHeight = 720;

    Gas(){};
    ~Gas() = default;

    void Load(const Chamber& chamber);
    void Render() const;
    void Populate(const Chamber& chamber);
    Vector2 Spawn(float radius, const Chamber& chamber);
    void Test();
    void Unload();
    void Update(const Chamber& chamber);
    // TODO: move physics simulation to a namespace
    void UpdateMovement(Molecule &mol, Vector2 force);
    void CheckBounds(Molecule &mol, const Chamber& chamber);
    void CheckCollision(Molecule &mol);
    void CollideZone(Molecule &mol);
    void Collide(Molecule &m1, Molecule *m2);
    void Repulse(Molecule &m1, Molecule *m2);
    float calculateShannonEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const;
    float calculateBoltzmannEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const;
};
