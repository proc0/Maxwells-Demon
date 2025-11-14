#pragma once

#include <raylib.h>
#include <raymath.h>

#define DENSITY 250
#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 200
#define CONTAINER_Y 200
#define GRAVITY 9.8f
#define RESTITUTION 1.0f

typedef struct Molecule {
    Vector2 force {};
    Vector2 origin {};
    Vector2 position {};
    Vector2 velocity {};
    Vector2 acceleration {};
    Color color;
    float mass = 0.0f;
    float radius = 0.0f;
    int id = 0;
    bool active = false;
    bool collided = false;
    int debounce = 60;
} Molecule;

class Gas {
    Molecule molecules[DENSITY];

    public:
        void Load();
        void Create();
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
