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
#define GRAVITY 9.8f
#define RESTITUTION 1.0f
#define MOLECULE_RADIUS 5.0f

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
    Vector2 cellCount;
    float cellSize;
    int queryIds = 0;

    public:
        void Load(int gridWidth, int gridHeight, float _cellSize);
        Vector2 place(const Molecule* mol) const;
        void add(Molecule* mol);
        void remove(Molecule* mol);
        void update(Molecule* mol);
        void clear();
        const std::vector<Molecule*>& getCell(int x, int y) const;
        std::vector<Molecule*> getZone(Molecule* mol);
};

class Gas {
    Molecule molecules[DENSITY];
    Grid grid;

    public:
        void Load();
        void Render() const;
        void Unload();
        void Update();
        void UpdateMovement(Molecule& mol);
        void CheckBounds(Molecule& mol);
        void CheckCollision(Molecule& mol);
        void CollideZone(Molecule& mol);
        void Collide(Molecule& m1, Molecule& m2);
        void Repulse(Molecule& m1, Molecule& m2);
};
