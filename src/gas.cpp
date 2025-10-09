#include "gas.hpp"

void Gas::Load() {
    Create();
}

void Gas::Create() {
    for(int i=0; i<DENSITY; i++){
        if(molecules[i].active) {
            continue;
        }
        
        molecules[i] = {
            .force = { x: 0.0f, y: 0.0f },
            .origin = { x: 0.0f, y: 0.0f },
            .position = { x: float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), y: float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) },
            .velocity = { x: 0.0f, y: 0.0f },
            .color = GetRandomValue(0, 1) == 1 ? RED : BLUE,
            .mass = 1.0f,
            .radius = 5.0f,
            .id = i,
            .active = true,
            .collided = false,
        };
    }
}

void Gas::Render() const {
    DrawRectangle(CONTAINER_X-3, CONTAINER_Y-3, CONTAINER_WIDTH+6, CONTAINER_HEIGHT+6, BLACK);
    DrawRectangle(CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH, CONTAINER_HEIGHT, RAYWHITE);

    for (const Molecule& mol : molecules) {
        if(!mol.active) continue;

        DrawCircle(mol.position.x, mol.position.y, mol.radius, mol.color);
    }
}

void Gas::Update() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        for (Molecule& mol : molecules) {
            mol.position = { x: float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), y: float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) };
        }
    }
}

void Gas::Unload() {
    
}