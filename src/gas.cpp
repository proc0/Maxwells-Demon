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
            // .force = {0, 0},
            .force = { float(GetRandomValue(-20, 20)), float(GetRandomValue(-20, 20)) },
            // .force = { GetRandomValue(0, 1) == 1 ? float(GetRandomValue(-500, 0)) : float(GetRandomValue(500, 0)), GRAVITY },
            .origin = { 0.0f, 0.0f },
            .position = { float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) },
            .velocity = { 0.0f, 0.0f },
            .acceleration = { 0.0f, 0.0f },
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
        UpdateMovement(mol);
        CheckBounds(mol);
        CollideZone(mol);
    }
}

void Gas::Unload() {
    
}

void Gas::CheckBounds(Molecule& mol) {
    
    if(mol.position.x + mol.radius > CONTAINER_WIDTH + CONTAINER_X - 3){
        mol.position.x = CONTAINER_WIDTH + CONTAINER_X - 3 - mol.radius;
        mol.velocity.x *= -RESTITUTION;
    } else if(mol.position.x - mol.radius < CONTAINER_X + 3) {
        mol.position.x = CONTAINER_X + 3 + mol.radius;
        mol.velocity.x *= -RESTITUTION;
        // mol.force.x -= mol.force.x/2;
    }

    if(mol.position.y + mol.radius > CONTAINER_HEIGHT + CONTAINER_Y - 3) {
        mol.position.y = CONTAINER_HEIGHT + CONTAINER_Y - 3 - mol.radius;
        mol.velocity.y *= -RESTITUTION;
    } else if(mol.position.y - mol.radius < CONTAINER_Y + 3) {
        mol.position.y = CONTAINER_Y + 3 + mol.radius;
        mol.velocity.y *= -RESTITUTION;
        // mol.force.y -= mol.force.y/2;
    }
}

void Gas::CheckCollision(Molecule &mol) {
    for(Molecule& other: molecules) {
        if(other.id == mol.id){
            continue;
        }

        if(CheckCollisionCircles(other.position, other.radius, mol.position, mol.radius)){
            Repulse(mol, other);
            other.collided = true;
            mol.collided = true;
        }
    }
}

void Gas::CollideZone(Molecule &mol) {
    if(!mol.collided) return;

    for(Molecule& other: molecules) {
        if(other.id == mol.id || !other.collided){
            continue;
        }

        Collide(mol, other);

        mol.collided = false;
        other.collided = false;
    }
}

void Gas::Collide(Molecule& m1, Molecule& m2) {
    Vector2 normal = m1.position - m2.position;
    Vector2 unitNormal = Vector2Normalize(normal);
    Vector2 unitTangent = Vector2(-unitNormal.y, unitNormal.x);

    float normalComponent1 = Vector2DotProduct(m1.velocity, unitNormal);
    float tangentComponent1 = Vector2DotProduct(m1.velocity, unitTangent);
    float normalComponent2 = Vector2DotProduct(m2.velocity, unitNormal);
    float tangentComponent2 = Vector2DotProduct(m2.velocity, unitTangent);

    float totalMass = m1.mass + m2.mass;
    float normalVelocity1 = (normalComponent1*(m1.mass - m2.mass) + 2*m2.mass*normalComponent2)/totalMass;
    float normalVelocity2 = (normalComponent2*(m2.mass - m1.mass) + 2*m1.mass*normalComponent1)/totalMass;

    Vector2 normalVectorVelocity1 = unitNormal * normalVelocity1;
    Vector2 normalVectorVelocity2 = unitNormal * normalVelocity2;
    Vector2 tangentVectorVelocity1 = unitTangent * tangentComponent1;
    Vector2 tangentVectorVelocity2 = unitTangent * tangentComponent2;

    m1.velocity = Vector2Add(normalVectorVelocity1, tangentVectorVelocity1);
    m2.velocity = Vector2Add(normalVectorVelocity2, tangentVectorVelocity2);
}

void Gas::Repulse(Molecule& m1, Molecule& m2){
    const float molDistance = Vector2Distance(m1.position, m2.position);
    const float collideDistance = m1.radius + m2.radius;

    const float deltaDistance = collideDistance - molDistance;

    const Vector2 normal1 = Vector2Normalize(Vector2Subtract(m1.position, m2.position));
    const Vector2 normal2 = Vector2Normalize(Vector2Subtract(m2.position, m1.position));
    const Vector2 repulse1 = normal1 * deltaDistance;
    const Vector2 repulse2 = normal2 * deltaDistance;

    m1.position += repulse1/(m1.mass*m1.mass);
    m2.position += repulse2/(m2.mass*m2.mass);
}

void Gas::UpdateMovement(Molecule &mol) {
    // Velocity Verlet Integration
    // (i)   x(t+Δt) = x(t) + v(t)Δt + 1/2a(t)Δt^2
    // (ii)  a(t+Δt) = f(x(t+Δt))
    // (iii) v(t+Δt) = v(t) + 1/2(a(t)+a(t+Δt))Δt

    // Δt and Δt * 1/2
    const float deltaTime = GetFrameTime();
    const float halfTimeSq = deltaTime * deltaTime * 0.5f;
    const Vector2 newVelocity = mol.velocity * deltaTime;
    const Vector2 newAcceleration = mol.acceleration * halfTimeSq;
    mol.position += Vector2Add(newVelocity, newAcceleration);

    CheckCollision(mol);

    if(mol.collided) return;

    mol.acceleration = newAcceleration;
    Vector2 nextAcceleration = mol.force/mol.mass;
    Vector2 halfStepVelocity = Vector2Add(mol.velocity, mol.acceleration*(deltaTime/2));
    mol.velocity = Vector2Add(halfStepVelocity, nextAcceleration*(deltaTime/2));
    // // [acceleration] using F=ma
    // const Vector2 acceleration = mol.force/mol.mass;
    // // (iii) [next velocity] without force change (skips (ii))
    // // v(t+Δt) = v(t) + a(t)Δt
    // mol.velocity = mol.velocity + acceleration * deltaTime;
    // // (i) [next position] expanding first equation to match order
    // // x(t+Δt) = x(t) + v(t) * Δt + a(t) * Δt * (Δt * 1/2)
    // mol.position = mol.position + mol.velocity * deltaTime + acceleration * deltaTime * halfTimeSq;

    // mol.force = mol.velocity/deltaTime;
    // [rotation] basic simple rotation effect
    // mol.rotation += acceleration.x * halfTime;
}
