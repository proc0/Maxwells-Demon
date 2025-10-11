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
            .debounce = 60,
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

        for(Molecule& m2: molecules) {
            if(m2.id == mol.id){
                continue;
            }

            if(CheckCollisionCircles(m2.position, m2.radius, mol.position, mol.radius)){
                // mol.collided = true;
                // m2.collided = true;
                // Vector2 vel = mol.velocity;
                // mol.velocity = Vector2Subtract(vel, m2.velocity)/2;
                // m2.velocity = Vector2Subtract(m2.velocity, vel)/2;
                float mRatio = (RESTITUTION + 1)*mol.mass/(mol.mass + m2.mass);
                Vector2 vDiff = Vector2Subtract(mol.velocity, m2.velocity);
                Vector2 pDiff = Vector2Subtract(mol.position, m2.position);


                // Vector2 proj = { 0 };

                // float v1dv2 = (vDiff.x*pDiff.x + vDiff.y*pDiff.y);
                // float v2dv2 = (pDiff.x*pDiff.x + pDiff.y*pDiff.y);

                // float mag = v1dv2/v2dv2;

                // proj.x = pDiff.x*mag;
                // proj.y = pDiff.y*mag;

                Vector2 proj = pDiff*(Vector2DotProduct(vDiff, pDiff)/Vector2DotProduct(pDiff, pDiff));
                mol.velocity = Vector2Subtract(mol.velocity, proj*mRatio);
                // mol.force = Vector2Subtract(mol.force, proj*mRatio);
                mol.force = Vector2SubtractValue(mol.force, 0.5f);
            }
        }

        // if(!mol.collided){
        //     mol.debounce = false;
        //     mol.debounce = false;
        // }

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

    // mol.force = mol.velocity/deltaTime;
    // [rotation] basic simple rotation effect
    // mol.rotation += acceleration.x * halfTime;
    
    if(mol.position.x + mol.radius > CONTAINER_WIDTH + CONTAINER_X - 3 || mol.position.x - mol.radius < CONTAINER_X + 3) {
        mol.velocity.x *= -RESTITUTION;
        mol.force.x -= mol.force.x/2;
    }

    if(mol.position.y + mol.radius > CONTAINER_HEIGHT + CONTAINER_Y - 3 || mol.position.y - mol.radius < CONTAINER_Y + 3) {
        mol.velocity.y *= -RESTITUTION;
        mol.force.y -= mol.force.y/2;
    }
    // on collision
    // if(mol.collided && mol.debounce == 60) {
    //     // reverse direction
    //     mol.velocity *= -1;
    //     mol.collided = false;
    //     mol.debounce--;
    // }

    // if(mol.debounce < 60 && mol.debounce > 0){
    //     mol.debounce--;
    // }

    // if(mol.debounce <= 0){
    //     mol.debounce = 60;
    //     mol.collided = false;
    // }

    // if(mol.velocity.y < 0 ) {
    //     // add gravity on bounce
    //     mol.velocity.y += GRAVITY * deltaTime;
    // } else {
    //     // reset debounce on fall
    //     mol.debounce = false;
    // }
}