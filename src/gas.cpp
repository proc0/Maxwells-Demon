#include "gas.hpp"
#include "raylib.h"

Thermal Gas::Load(const Chamber& chamber, short density) {
    this->density = density;
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS * 4);
    return Populate(chamber);
}

Thermal Gas::Populate(const Chamber& chamber) {

    // float leftChamberHotCount = 0;
    // float leftChamberCoolCount = 0;
    // float rightChamberHotCount = 0;
    // float rightChamberCoolCount = 0;
    float totalHotCount = 0;
    // float totalCoolCount = 0;
    short densityCount = density < MAX_DENSITY ? density : MAX_DENSITY;
    short maxHotCount = densityCount/2;

    for (Molecule& mol : molecules) {
        mol.active = false;
    }

    for (short i = 0; i < densityCount; i++) {
        bool isHot = true;
        if(totalHotCount > maxHotCount) {
            isHot = false;
        }

        Color color1 = isHot ? BEIGE : DARKBLUE;
        Color color2 = isHot ? RED : BLUE;

        float randNegX = float(GetRandomValue(-300, -200));
        float randPosX = float(GetRandomValue(200, 300));
        float randNegY = float(GetRandomValue(-300, -200));
        float randPosY = float(GetRandomValue(200, 300));
        Vector2 randForce = Vector2(GetRandomValue(0, 1) == 1 ? randNegX : randPosX, GetRandomValue(0, 1) == 1 ? randNegY : randPosY);
        
        molecules[i] = {
            .force = isHot ? randForce : ZERO_VECTOR,
            .origin = ZERO_VECTOR,
            .position = Spawn(MOLECULE_RADIUS, chamber),
            .velocity = ZERO_VECTOR,
            .acceleration = ZERO_VECTOR,
            .color1 = color1,
            .color2 = color2,
            .color = color1,
            .mass = 2.0f,
            .radius = MOLECULE_RADIUS,
            .restitution = isHot ? 1.0f : RESTITUTION,
            .id = i,
            .active = true,
            .collided = false,
            .isHot = isHot
        };

        grid.AddPoint(molecules[i].position, i);

        if (molecules[i].isHot) {
            totalHotCount++;
            if (chamber.IsLeft(molecules[i].position, molecules[i].radius)) {
                molecules[i].isLeft = true;
                // leftChamberHotCount++;
            } else {
                // rightChamberHotCount++;
                molecules[i].isLeft = false;
            }
        } else {
            // totalCoolCount++;
            if (chamber.IsLeft(molecules[i].position, molecules[i].radius)) {
                molecules[i].isLeft = true;
                // leftChamberCoolCount++;
            } else {
                // rightChamberCoolCount++;
                molecules[i].isLeft = false;
            }
        }
    }

    return Update(chamber);
    // return {
    //     .leftHot = leftChamberHotCount,
    //     .leftCold = leftChamberCoolCount,
    //     .rightHot = rightChamberHotCount,
    //     .rightCold = rightChamberCoolCount,
    // };
}

void Gas::Add(const Chamber& chamber) {
    short newDensity = density + 1;
    if (newDensity >= MAX_DENSITY) return;

    density = newDensity;

    bool isHot = GetRandomValue(0, 1) == 1;
    bool isLeft = GetRandomValue(0, 1) == 1;

    Color color1 = isHot ? BEIGE : DARKBLUE;
    Color color2 = isHot ? RED : BLUE;

    float randNegX = float(GetRandomValue(-300, -200));
    float randPosX = float(GetRandomValue(200, 300));
    float randNegY = float(GetRandomValue(-300, -200));
    float randPosY = float(GetRandomValue(200, 300));
    Vector2 randForce = Vector2(GetRandomValue(0, 1) == 1 ? randNegX : randPosX, GetRandomValue(0, 1) == 1 ? randNegY : randPosY);
    
    molecules[density] = {
        .force = isHot ? randForce : ZERO_VECTOR,
        .origin = ZERO_VECTOR,
        .position = Spawn(MOLECULE_RADIUS, chamber),
        .velocity = ZERO_VECTOR,
        .acceleration = ZERO_VECTOR,
        .color1 = color1,
        .color2 = color2,
        .color = color1,
        .mass = 2.0f,
        .radius = MOLECULE_RADIUS,
        .restitution = isHot ? 1.0f : RESTITUTION,
        .id = density,
        .active = true,
        .collided = false,
        .isHot = isHot,
        .isLeft = isLeft,
    };

    grid.AddPoint(molecules[density].position, density);
}

void Gas::Sub(const Chamber& chamber) {
    if (density < 1) return;
    // TODO: check hot and cold balance to choose which to remove
    molecules[density-1].active = false;
    density--;
}

Thermal Gas::Count() const {
    Thermal stats = {0};
    for (const Molecule &mol : molecules) {
        if (!mol.active) continue;
        ThermalCount(mol, stats);
    }

    return stats;
}

void Gas::ThermalCount(const Molecule& mol, Thermal& stats) const {
    if (mol.isHot && mol.isLeft) {
        stats.leftHot++;
    } else if (mol.isHot && !mol.isLeft) {
        stats.rightHot++;
    } else if (!mol.isHot && mol.isLeft) {
        stats.leftCold++;
    } else if (!mol.isHot && !mol.isLeft) {
        stats.rightCold++;
    }
}

Vector2 Gas::Spawn(float radius, const Chamber& chamber)
{
    Vector2 position = {
        float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH)), 
        float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT))
    };
    
    int retries = 12;
    while (retries >= 0)
    {
        if (!chamber.checkTunneling(position, radius))
            break;

        position = {
            float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH)), 
            float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT))
        };
        
        retries--;
    }

    if (chamber.checkTunneling(position, radius))
    {
        position = {CONTAINER_X + 10, CONTAINER_Y + 10};
    }

    return position;
}

void Gas::Render() const {
    for (const Molecule &mol : molecules) {
        if (!mol.active)
            continue;

        DrawCircle(mol.position.x, mol.position.y, mol.radius, mol.color);
    }
}

Thermal Gas::Update(const Chamber& chamber)
{
    for (Molecule &mol : molecules)
    {
        if (!mol.active) continue;

        CheckBounds(mol, chamber);
        CollideZone(mol);
        mol.cell = grid.UpdatePoint(mol.position, mol.cell, mol.id);
    }

    Thermal stats = {0};

    for (Molecule &mol : molecules)
    {
        if (!mol.active) continue;

        CheckBounds(mol, chamber);
        UpdateMovement(mol, ZERO_VECTOR);
        CollideZone(mol);
        mol.cell = grid.UpdatePoint(mol.position, mol.cell, mol.id);
        ThermalCount(mol, stats);
    }

    // grid.CacheClear();

    return stats;
}

void Gas::Unload()
{
}

void Gas::CheckBounds(Molecule &mol, const Chamber& chamber)
{

    Cell wallZone = chamber.GetWalls(mol.position, mol.radius);

    for (auto cid : wallZone) {
        const Wall& wall = chamber.GetWall(cid);
        const Rectangle& wallRect = wall.rect;

        if (CheckCollisionCircleRec(mol.position, mol.radius, wallRect)) {
            if (Locate::Right(mol.position, mol.radius) > wallRect.x && mol.position.x < wallRect.x && mol.velocity.x > 0) {
                mol.position.x = wallRect.x - mol.radius - 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            } else if (Locate::Left(mol.position, mol.radius) < wallRect.x + wallRect.width && mol.position.x > wallRect.x && mol.velocity.x < 0) {
                mol.position.x = wallRect.x + wallRect.width + mol.radius + 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            } else if (mol.position.x + mol.radius > wallRect.width + wallRect.x) {
                mol.velocity.x *= -mol.restitution;
            } else if (mol.position.x - mol.radius < wallRect.x) {
                mol.velocity.x *= -mol.restitution;
            } else if (mol.position.y + mol.radius > wallRect.height + wallRect.y) {
                mol.velocity.y *= -mol.restitution;
            } else if (mol.position.y - mol.radius < wallRect.y) {
                mol.velocity.y *= -mol.restitution;
            }
        }
    }

    if (mol.position.x + mol.radius > CONTAINER_WIDTH + CONTAINER_X) {
        mol.position.x = CONTAINER_WIDTH + CONTAINER_X - mol.radius;
        mol.velocity.x *= -mol.restitution;
        mol.force.x *= -mol.restitution;
    } else if (mol.position.x - mol.radius < CONTAINER_X) {
        mol.position.x = CONTAINER_X + mol.radius;
        mol.velocity.x *= -mol.restitution;
        mol.force.x *= -mol.restitution;
    }

    if (mol.position.y + mol.radius > CONTAINER_HEIGHT + CONTAINER_Y) {
        mol.position.y = CONTAINER_HEIGHT + CONTAINER_Y - mol.radius;
        mol.velocity.y *= -mol.restitution;
        mol.force.y *= -mol.restitution;
    } else if (mol.position.y - mol.radius < CONTAINER_Y) {
        mol.position.y = CONTAINER_Y + mol.radius;
        mol.velocity.y *= -mol.restitution;
        mol.force.y *= -mol.restitution;
    }

    if (chamber.IsDetected(mol.position, mol.radius)) {
        mol.isLeft = chamber.IsLeft(mol.position, mol.radius);
    }
}

void Gas::CheckCollision(Molecule &mol) {
    Cell zone = grid.ZonePoint(mol.position, mol.radius, mol.id);

    for (auto cid : zone) {
        if (cid == mol.id) continue;

        Molecule* other = &molecules[cid];

        if (CheckCollisionCircles(other->position, other->radius, mol.position, mol.radius)) {
            Repulse(mol, other);
            other->collided = true;
            mol.collided = true;
        }
    }
}

void Gas::CollideZone(Molecule &mol) {
    if (!mol.collided) return;

    Cell zone = grid.ZonePoint(mol.position, mol.radius, mol.id);

    for (auto cid : zone) {
        Molecule* other = &molecules[cid];

        if (cid == mol.id || !other->collided) continue;

        Collide(mol, other);
    }
}

void Gas::Collide(Molecule &m1, Molecule *m2) {
    Vector2 normal = m1.position - m2->position;
    Vector2 unitNormal = Vector2Normalize(normal);
    Vector2 unitTangent = Vector2(-unitNormal.y, unitNormal.x);

    float normalComponent1 = Vector2DotProduct(m1.velocity, unitNormal);
    float tangentComponent1 = Vector2DotProduct(m1.velocity, unitTangent);
    float normalComponent2 = Vector2DotProduct(m2->velocity, unitNormal);
    float tangentComponent2 = Vector2DotProduct(m2->velocity, unitTangent);

    float totalMass = m1.mass + m2->mass;
    float normalVelocity1 = (m1.restitution * normalComponent1 * (m1.mass - m2->mass) + 2.0f * m2->mass * normalComponent2) / totalMass;
    float normalVelocity2 = (m2->restitution * normalComponent2 * (m2->mass - m1.mass) + 2.0f * m1.mass * normalComponent1) / totalMass;

    Vector2 normalVectorVelocity1 = unitNormal * normalVelocity1;
    Vector2 normalVectorVelocity2 = unitNormal * normalVelocity2;
    Vector2 tangentVectorVelocity1 = unitTangent * tangentComponent1;
    Vector2 tangentVectorVelocity2 = unitTangent * tangentComponent2;

    m1.velocity = Vector2Add(normalVectorVelocity1, tangentVectorVelocity1);
    m2->velocity = Vector2Add(normalVectorVelocity2, tangentVectorVelocity2);

    // reverse force for hot molecules according to impact normal
    Vector2 signedNormal = Vector2(unitNormal.x > 0 ? 1 : -1, unitNormal.y > 0 ? 1 : -1);
    if(m1.isHot) {
        m1.force *= signedNormal;
    }

    if(m2->isHot) {
        m2->force *= signedNormal;
    }

    m1.collided = false;
    m2->collided = false;
}

void Gas::Repulse(Molecule &m1, Molecule *m2) {
    const float molDistance = Vector2Distance(m1.position, m2->position);
    const float collideDistance = m1.radius + m2->radius;

    const float deltaDistance = collideDistance - molDistance;

    const Vector2 normal1 = Vector2Normalize(Vector2Subtract(m1.position, m2->position));
    const Vector2 normal2 = Vector2Normalize(Vector2Subtract(m2->position, m1.position));
    const Vector2 repulse1 = normal1 * deltaDistance;
    const Vector2 repulse2 = normal2 * deltaDistance;

    m1.position += repulse1 / m1.mass;
    m2->position += repulse2 / m2->mass;
}

void Gas::UpdateMovement(Molecule &mol, Vector2 force) {
    // Velocity Verlet Integration
    // (i)   x(t+Δt) = x(t) + v(t)Δt + 1/2a(t)Δt^2
    // (ii)  a(t+Δt) = f(x(t+Δt))
    // (iii) v(t+Δt) = v(t) + 1/2(a(t)+a(t+Δt))Δt
    const float deltaTime = GetFrameTime();
    const float halfTimeSq = deltaTime * deltaTime * 0.5f;
    const Vector2 newVelocity = mol.velocity * deltaTime;
    const Vector2 newAcceleration = mol.acceleration * halfTimeSq;
    mol.position += Vector2Add(newVelocity, newAcceleration);

    CheckCollision(mol);

    if (mol.collided)
        return;

    mol.acceleration = newAcceleration;
    Vector2 nextAcceleration = (force + mol.force) / mol.mass;
    Vector2 halfStepVelocity = Vector2Add(mol.velocity, mol.acceleration * (deltaTime / 2));
    mol.velocity = Vector2Add(halfStepVelocity, nextAcceleration * (deltaTime / 2));

    // speed limit
    if (mol.velocity.x > MAX_SPEED) {
        mol.velocity.x = MAX_SPEED;
    }

    if (mol.velocity.x < -MAX_SPEED) {
        mol.velocity.x = -MAX_SPEED;
    }

    if (mol.velocity.y > MAX_SPEED) {
        mol.velocity.y = MAX_SPEED;
    }

    if (mol.velocity.y < -MAX_SPEED) {
        mol.velocity.y = -MAX_SPEED;
    }

    // color update
    Vector2 normalVel = Vector2Normalize(newVelocity);
    mol.color = ColorLerp(mol.color1, mol.color2, INVERSE_EXPONENTIAL((normalVel.x + normalVel.y) / 10.0f));
}

