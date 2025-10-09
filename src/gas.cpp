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
            .force = { float(GetRandomValue(-20, 20)), float(GetRandomValue(-20, 20)) },
            // .force = { GetRandomValue(0, 1) == 1 ? float(GetRandomValue(-500, 0)) : float(GetRandomValue(500, 0)), GRAVITY },
            .origin = { 0.0f, 0.0f },
            .position = { float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) },
            .velocity = { 0.0f, 0.0f },
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
    // if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
    //     for (Molecule& mol : molecules) {
    //         mol.position = { x: float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), y: float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) };
    //     }
    // }
    for (Molecule& mol : molecules) {
        UpdateMovement(mol);
    }
}

void Gas::Unload() {
    
}

void Gas::UpdateMovement(Molecule &mol) {
    // Velocity Verlet Integration
    // (i)   x(t+Δt) = x(t) + v(t)Δt + 1/2a(t)Δt^2
    // (ii)  a(t+Δt) = f(x(t+Δt))
    // (iii) v(t+Δt) = v(t) + 1/2(a(t)+a(t+Δt))Δt

    // Δt and Δt * 1/2
    const float deltaTime = GetFrameTime();
    const float halfTime = deltaTime * 0.5f;
    // [acceleration] using F=ma
    const Vector2 acceleration = mol.force/mol.mass;
    // (i) [next position] expanding first equation to match order
    // x(t+Δt) = x(t) + v(t) * Δt + a(t) * Δt * (Δt * 1/2)
    mol.position = mol.position + mol.velocity * deltaTime + acceleration * deltaTime * halfTime;
    // (iii) [next velocity] without force change (skips (ii))
    // v(t+Δt) = v(t) + a(t)Δt
    mol.velocity = mol.velocity + acceleration * deltaTime;

    // [rotation] basic simple rotation effect
    // mol.rotation += acceleration.x * halfTime;
    
    if(mol.position.x > CONTAINER_WIDTH + CONTAINER_X || mol.position.x < CONTAINER_X || mol.position.y > CONTAINER_HEIGHT + CONTAINER_Y || mol.position.y < CONTAINER_Y) {
        mol.velocity *= -1;
    }
    // on collision
    // if(mol.collided && !mol.debounce) {
    //     // reverse direction
    //     mol.velocity *= -1;
    //     mol.collided = false;
    //     mol.debounce = true;
    // }

    // if(mol.velocity.y < 0 ) {
    //     // add gravity on bounce
    //     mol.velocity.y += GRAVITY * deltaTime;
    // } else {
    //     // reset debounce on fall
    //     mol.debounce = false;
    // }
}