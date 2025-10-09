#pragma once

#include <raylib.h>

#define DENSITY 50
#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 200
#define CONTAINER_Y 200

typedef struct Molecule {
    Vector2 force {};
    Vector2 origin {};
    Vector2 position {};
    Vector2 velocity {};
    Color color;
    float mass = 0.0f;
    float radius = 0.0f;
    int id = 0;
    bool active = false;
    bool collided = false;
    // bool debounce = false;
} Molecule;

class Gas {
    Molecule molecules[DENSITY];

    public:
        void Load();
        void Create();
        void Render() const;
        void Unload();
        void Update();
};
