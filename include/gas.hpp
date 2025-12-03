#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <algorithm>

#define DENSITY 30
#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 200
#define CONTAINER_Y 200
#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60
#define MAX_SPEED 500.0f
#define RESTITUTION 0.8f
#define MOLECULE_RADIUS 12.0f
#define CONTAINER_RIGHT (CONTAINER_X + CONTAINER_WIDTH - 3)
#define CONTAINER_BOTTOM (CONTAINER_Y + CONTAINER_HEIGHT - 3)
#define GRAVITY Vector2(0, 982.0f)
#define ZERO_FORCE Vector2(0, 0)

#define EASE_OUT_EXPO(x) (x >= 1 ? 1 : 1 - pow(2, -10 * x))


typedef struct Molecule {
    Vector2 force {};
    Vector2 origin {};
    Vector2 position {};
    Vector2 velocity {};
    Vector2 acceleration {};
    Vector2 cell {};
    Color color1;
    Color color2;
    Color color;
    float mass = 0.0f;
    float radius = 0.0f;
    float restitution = RESTITUTION;
    int id = 0;
    int queryId = 0;
    bool active = false;
    bool collided = false;
    bool isHot = false;

    float getLeft()   const { return position.x - radius; }
    float getRight()  const { return position.x + radius; }
    float getTop()    const { return position.y - radius; }
    float getBottom() const { return position.y + radius; }
} Molecule;

typedef struct Wall {
    Rectangle rect;
    std::vector<Vector2> cells;
    int id;
} Wall;

class Grid {
    std::vector<std::vector<std::vector<Molecule*>>> cells;
    std::vector<std::vector<std::vector<Wall*>>> wallCells;
    std::vector<Wall> walls;
    Vector2 cellCount;
    float cellSize;
    int queryIds = 0;
    int doorFrame = 0;
    bool isDoorClosing = false;

    public:
        void Load(int gridWidth, int gridHeight, float _cellSize);
        Vector2 place(float x, float y) const;
        void add(Molecule* mol);
        void addWalls(std::vector<Rectangle>& wallRects);
        void addWall(Wall* wall);
        void removeWall(Wall* wall);
        bool checkTunneling(Vector2 position, float radius);
        void remove(Molecule* mol);
        void update(Molecule* mol);
        void updateWall(Wall* wall);
        void updateWalls();
        void Render() const;
        void clear();
        std::vector<Molecule*> getZone(Molecule* mol);
        std::vector<Wall*> getWalls(Molecule* mol);
};

class Gas {
    Grid grid;
    Molecule molecules[DENSITY];
    std::vector<Rectangle> wallRects = { Rectangle({ 593, 200, 15, 150 }), Rectangle({ 593, 350, 15, 100 }), Rectangle({ 593, 450, 15, 150 }) };

    public:
        void Load();
        void Render() const;
        void Populate();
        Vector2 Spawn(float radius);
        void Test();
        void Unload();
        void Update();
        void UpdateMovement(Molecule& mol, Vector2 force);
        void CheckBounds(Molecule& mol);
        void CheckCollision(Molecule& mol);
        void CollideZone(Molecule& mol);
        void Collide(Molecule& m1, Molecule* m2);
        void Repulse(Molecule& m1, Molecule* m2);
};
