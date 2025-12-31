#pragma once

#include <array>

#include "common.hpp" // IWYU pragma: keep
#include "grid.hpp"
#include "chamber.hpp"

#define DENSITY 40

// TODO: expose MOLECULE_RAIDUS globally to resize grid accordingly
#define MAX_SPEED 500.0f
#define RESTITUTION 0.8f
#define MOLECULE_RADIUS 12.0f
#define CONTAINER_RIGHT (CONTAINER_X + CONTAINER_WIDTH - 3)
#define CONTAINER_BOTTOM (CONTAINER_Y + CONTAINER_HEIGHT - 3)
#define GRAVITY Vector2(0.0f, 982.0f)
#define ZERO_VECTOR Vector2(0.0f, 0.0f)

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


// typedef struct Wall
// {
//     Rectangle rect;
//     std::vector<Vector2> cells;
//     int id;
// } Wall;

class Gas
{
    std::array<Molecule, DENSITY> molecules;
    Grid grid;

    // Color barColor;
    // const char *pipeText = ":";

    // float completion = 0.0f;
    // float entropy = 0.0f;
    // float maxEntropy = 0.0f;
    // int entropyBarLength = CONTAINER_BORDER_WIDTH - 100;
    // int entropyBar = 0;
    // int entropyBarX = CONTAINER_BORDER_X + 50;
    // int entropyBarY = 40;

public:
    int screenWidth = 1280;
    int screenHeight = 720;
    float rightChamberHotCount = 0;
    float rightChamberCoolCount = 0;
    float leftChamberHotCount = 0;
    float leftChamberCoolCount = 0;
    float totalHotCount = 0;
    float totalCoolCount = 0;

    Gas(){};
    ~Gas() = default;

    ThermalCount Load(const Chamber& chamber);
    void Render() const;
    ThermalCount Populate(const Chamber& chamber);
    Vector2 Spawn(float radius, const Chamber& chamber);
    void Test();
    void Unload();
    ThermalCount Update(const Chamber& chamber);
    // TODO: move physics simulation to a namespace
    void UpdateMovement(Molecule &mol, Vector2 force);
    void CheckBounds(Molecule &mol, const Chamber& chamber);
    void CheckCollision(Molecule &mol);
    void CollideZone(Molecule &mol);
    void Collide(Molecule &m1, Molecule *m2);
    void Repulse(Molecule &m1, Molecule *m2);

};
