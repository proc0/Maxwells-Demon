#pragma once

#include <array>

#include "common.hpp"
#include "grid.hpp"
#include "chamber.hpp"

#define DENSITY 40

#define MAX_SPEED 500.0f
#define RESTITUTION 0.8f

#define GRAVITY Vector2(0.0f, 982.0f)
#define ZERO_VECTOR Vector2(0.0f, 0.0f)

typedef struct Molecule {
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
} Molecule;

class Gas {
    std::array<Molecule, DENSITY> molecules;
    Grid grid;

public:
    Gas(){};
    ~Gas() = default;

    ThermalCount Load(const Chamber& chamber);
    ThermalCount Populate(const Chamber& chamber);
    ThermalCount Update(const Chamber& chamber);
    Vector2 Spawn(float radius, const Chamber& chamber);
    void Render() const;

    void CheckBounds(Molecule &mol, const Chamber& chamber);
    void UpdateMovement(Molecule &mol, Vector2 force);
    void CheckCollision(Molecule &mol);
    void CollideZone(Molecule &mol);
    void Collide(Molecule &m1, Molecule *m2);
    void Repulse(Molecule &m1, Molecule *m2);
    
    void Unload();
};
