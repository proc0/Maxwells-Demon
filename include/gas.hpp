#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <algorithm>

#define DENSITY 50
#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 200
#define CONTAINER_Y 200
#define RESTITUTION 0.8f
#define MOLECULE_RADIUS 8.0f
#define CONTAINER_RIGHT (CONTAINER_X + CONTAINER_WIDTH - 3)
#define CONTAINER_BOTTOM (CONTAINER_Y + CONTAINER_HEIGHT - 3)
#define GRAVITY Vector2(0, 982.0f)
#define ZERO_FORCE Vector2(0, 0)

typedef struct Molecule {
    Vector2 force {};
    Vector2 origin {};
    Vector2 position {};
    Vector2 velocity {};
    Vector2 acceleration {};
    Vector2 cell {};
    Color color;
    float mass = 0.0f;
    float radius = 0.0f;
    int id = 0;
    bool active = false;
    bool collided = false;
    int debounce = 60;
    int queryId = 0;

    float getLeft()   const { return position.x - radius; }
    float getRight()  const { return position.x + radius; }
    float getTop()    const { return position.y - radius; }
    float getBottom() const { return position.y + radius; }
} Molecule;

class Grid {
    std::vector<std::vector<std::vector<Molecule*>>> cells;
    std::vector<std::vector<bool>> walls;
    Vector2 cellCount;
    float cellSize;
    int queryIds = 0;

    public:
        void Load(int gridWidth, int gridHeight, float _cellSize);
        Vector2 place(float x, float y) const;
        void add(Molecule* mol);
        void addWall(Rectangle& wall);
        void remove(Molecule* mol);
        void update(Molecule* mol);
        void Render() const;
        void clear();
        std::vector<Molecule*> getZone(Molecule* mol);
        bool getWalls(Molecule* mol);
};

class Gas {
    Molecule molecules[DENSITY];
    Rectangle wallRect = { 400, 200, 20, 200 };
    Grid grid;

    public:
        void Load();
        void Render() const;
        void Populate();
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
