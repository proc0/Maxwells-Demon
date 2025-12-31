#include "gas.hpp"
#include "common.hpp"
#include "raylib.h"

ThermalCount Gas::Load(const Chamber& chamber)
{

    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS * 4);
    return Populate(chamber);
}

void Gas::Test()
{

    molecules[0] = {
        .force = {10, 0},
        .origin = {0.0f, 0.0f},
        .position = {CONTAINER_X + 250, CONTAINER_Y + 200},
        .velocity = {0.0f, 0.0f},
        .acceleration = {0.0f, 0.0f},
        .color = RED,
        .mass = 1.0f,
        .radius = MOLECULE_RADIUS,
        .id = 0,
        .active = true,
        .collided = false,
    };

    grid.addPoint(molecules[0].position, 0);

    molecules[1] = {
        .force = {-10, 5},
        .origin = {0.0f, 0.0f},
        .position = {CONTAINER_X + 450, CONTAINER_Y + 150},
        .velocity = {0.0f, 0.0f},
        .acceleration = {0.0f, 0.0f},
        .color = BLUE,
        .mass = 1.0f,
        .radius = MOLECULE_RADIUS,
        .id = 1,
        .active = true,
        .collided = false,
    };

    grid.addPoint(molecules[1].position, 1);
}

ThermalCount Gas::Populate(const Chamber& chamber)
{
    // grid.addWalls(chamber.walls);
    // grid.addWall(chamber.walls[0]);
    // grid.addWall(chamber.walls[1]);
    // grid.addWall(chamber.walls[2]);

    int maxMoleculeType = DENSITY/2;
    for (short i = 0; i < DENSITY; i++)
    {
        if (molecules[i].active)
        {
            continue;
        }

        Vector2 vel = {
            float(GetRandomValue(-100, 100)), 
            float(GetRandomValue(-100, 100))
        };

        bool isHot = GetRandomValue(0, 1) == 1;
        if(isHot && totalHotCount > maxMoleculeType) {
            isHot = false;
        } else if (!isHot && totalCoolCount > maxMoleculeType) {
            isHot = true;
        }

        Color color1 = isHot ? BEIGE : DARKBLUE;
        Color color2 = isHot ? RED : BLUE;

        float randNegX = float(GetRandomValue(-250, -150));
        float randNegY = float(GetRandomValue(-250, -150));
        float randPosX = float(GetRandomValue(150, 20));
        float randPosY = float(GetRandomValue(150, 250));
        Vector2 randForce = Vector2(GetRandomValue(0, 1) == 1 ? randNegX : randPosX, GetRandomValue(0, 1) == 1 ? randNegY : randPosY);
        molecules[i] = {
            // .force = { GetRandomValue(0, 1) == 1 ? float(GetRandomValue(-500, 0)) : float(GetRandomValue(500, 0)), GRAVITY },
            .force = isHot ? randForce : ZERO_VECTOR,
            .origin = ZERO_VECTOR,
            .position = Spawn(MOLECULE_RADIUS, chamber),
            // .velocity = vel,
            .velocity = ZERO_VECTOR,
            .acceleration = ZERO_VECTOR,
            .color1 = color1,
            .color2 = color2,
            .color = ColorLerp(color1, color2, fabsf(vel.x) + fabsf(vel.y)),
            .mass = 1.0f,
            .radius = MOLECULE_RADIUS,
            .restitution = isHot ? 1.0f : RESTITUTION,
            .id = i,
            .active = true,
            .collided = false,
            .isHot = isHot
        };

        grid.addPoint(molecules[i].position, i);

        if (molecules[i].isHot)
        {
            totalHotCount++;
            if (molecules[i].position.x > static_cast<float>(screenWidth)/2.0f)
            {
                rightChamberHotCount++;
            }
            else
            {
                molecules[i].isLeft = true;
                leftChamberHotCount++;
            }
        }
        else
        {
            totalCoolCount++;
            if (molecules[i].position.x > static_cast<float>(screenWidth)/2.0f)
            {
                rightChamberCoolCount++;
            }
            else
            {
                molecules[i].isLeft = true;
                leftChamberCoolCount++;
            }
        }
    }

    return {
        .leftHot = leftChamberHotCount,
        .leftCold = leftChamberCoolCount,
        .rightHot = rightChamberHotCount,
        .rightCold = rightChamberCoolCount,
    };
    // float halfHotCount = totalHotCount/2.0f;
    // float halfColdCount = totalCoolCount/2.0f;
    // maxEntropy = calculateBoltzmannEntropy(halfHotCount, totalHotCount - halfHotCount, halfColdCount, totalCoolCount - halfColdCount);
    // entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    // completion = entropy / maxEntropy;
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

void Gas::Render() const
{


    // grid.Render();

    for (const Molecule &mol : molecules)
    {
        if (!mol.active)
            continue;

        DrawCircle(mol.position.x, mol.position.y, mol.radius, mol.color);
    }

    // const char *leftHotCountText = TextFormat("%.f", leftChamberHotCount);
    // const char *leftColdCountText = TextFormat("%.f", leftChamberCoolCount);
    // const char *rightHotCountText = TextFormat("%.f", rightChamberHotCount);
    // const char *rightColdCountText = TextFormat("%.f", rightChamberCoolCount);

    // DrawText(leftHotCountText, 280, 15, 20, RED);
    // DrawText(pipeText, 300, 15, 20, BLACK); 
    // DrawText(leftColdCountText, 308, 15, 20, BLUE);

    // DrawText(rightHotCountText, 948, 15, 20, RED);
    // DrawText(pipeText, 970, 15, 20, BLACK);
    // DrawText(rightColdCountText, 980, 15, 20, BLUE);

    // const char *entText = TextFormat("Entropy %.2f", entropy);
    // const char *maxEntText = TextFormat("Max %.2f", maxEntropy);
    // DrawText(entText, 430, 15, 20, BLACK);
    // DrawText(maxEntText, 730, 15, 20, BLACK);

    // DrawRectangle(entropyBarX-1, entropyBarY-1, entropyBarLength+2, 7, BLACK);
    // DrawRectangle(entropyBarX, entropyBarY, entropyBarLength, 5, RAYWHITE);
    // DrawRectangle(entropyBarX, entropyBarY, entropyBar, 5, barColor);

}

ThermalCount Gas::Update(const Chamber& chamber)
{
    for (Molecule &mol : molecules)
    {
        CheckBounds(mol, chamber);
        UpdateMovement(mol, ZERO_VECTOR);
        CollideZone(mol);
        mol.cell = grid.update(mol.position, mol.cell, mol.id);
    }

    float _rightChamberHotCount = 0.0f;
    float _rightChamberCoolCount = 0.0f;
    float _leftChamberHotCount = 0.0f;
    float _leftChamberCoolCount = 0.0f;
    for (Molecule &mol : molecules)
    {
        if (mol.isHot && mol.isLeft)
        {
            _leftChamberHotCount++;
        }
        else if (mol.isHot && !mol.isLeft)
        {
            _rightChamberHotCount++;
        }
        else if (!mol.isHot && mol.isLeft)
        {
            _leftChamberCoolCount++;
        }
        else if (!mol.isHot && !mol.isLeft)
        {
            _rightChamberCoolCount++;
        }
    }
    rightChamberHotCount = _rightChamberHotCount;
    rightChamberCoolCount = _rightChamberCoolCount;
    leftChamberHotCount = _leftChamberHotCount;
    leftChamberCoolCount = _leftChamberCoolCount;

    return {
        .leftHot = leftChamberHotCount,
        .leftCold = leftChamberCoolCount,
        .rightHot = rightChamberHotCount,
        .rightCold = rightChamberCoolCount,
    };

    // grid.updateWalls();

    // entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    // completion = entropy / maxEntropy;
    // entropyBar = entropyBarLength * completion;
    // barColor = ColorLerp(GREEN, GRAY, completion);

    // if(leftChamberCoolCount == leftChamberHotCount) {
    //     colorChamberLeft = RAYWHITE;
    // } else {
    //     colorChamberLeft = Fade(leftChamberCoolCount > leftChamberHotCount ? ColorLerp(RAYWHITE, BLUE, leftChamberCoolCount/totalCoolCount) : ColorLerp(RAYWHITE, RED, leftChamberHotCount/totalHotCount), 0.2f);
    // }

    // if(rightChamberCoolCount == rightChamberHotCount) {
    //     colorChamberRight = RAYWHITE;
    // } else {
    //     colorChamberRight = Fade(rightChamberCoolCount > rightChamberHotCount ? ColorLerp(RAYWHITE, BLUE, rightChamberCoolCount/totalCoolCount) : ColorLerp(RAYWHITE, RED, rightChamberHotCount/totalHotCount), 0.2f);
    // }

}

void Gas::Unload()
{
}

void Gas::CheckBounds(Molecule &mol, const Chamber& chamber)
{

    Cell wallZone = chamber.GetWalls(mol.position, mol.radius);

    for (auto cid : wallZone)
    {
        // TODO: make walls private in chamber, expose another method
        const Wall& wall = chamber.GetWall(cid);
        const Rectangle& wallRect = wall.rect;
        if (CheckCollisionCircleRec(mol.position, mol.radius, wallRect))
        {
            if (Locate::Right(mol.position, mol.radius) > wallRect.x && mol.position.x < wallRect.x && mol.velocity.x > 0)
            {
                mol.position.x = wallRect.x - mol.radius - 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            }
            else if (Locate::Left(mol.position, mol.radius) < wallRect.x + wallRect.width && mol.position.x > wallRect.x && mol.velocity.x < 0)
            {
                mol.position.x = wallRect.x + wallRect.width + mol.radius + 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            }
            else if (mol.position.x + mol.radius > wallRect.width + wallRect.x)
            {
                mol.velocity.x *= -mol.restitution;
            }
            else if (mol.position.x - mol.radius < wallRect.x)
            {
                mol.velocity.x *= -mol.restitution;
            }
            else if (mol.position.y + mol.radius > wallRect.height + wallRect.y)
            {
                mol.velocity.y *= -mol.restitution;
            }
            else if (mol.position.y - mol.radius < wallRect.y)
            {
                mol.velocity.y *= -mol.restitution;
            }
        }
    }

    if (mol.position.x + mol.radius > CONTAINER_WIDTH + CONTAINER_X)
    {
        mol.position.x = CONTAINER_WIDTH + CONTAINER_X - mol.radius;
        mol.velocity.x *= -mol.restitution;
        mol.force.x *= -mol.restitution;
    }
    else if (mol.position.x - mol.radius < CONTAINER_X)
    {
        mol.position.x = CONTAINER_X + mol.radius;
        mol.velocity.x *= -mol.restitution;
        mol.force.x *= -mol.restitution;
    }

    if (mol.position.y + mol.radius > CONTAINER_HEIGHT + CONTAINER_Y)
    {
        mol.position.y = CONTAINER_HEIGHT + CONTAINER_Y - mol.radius;
        mol.velocity.y *= -mol.restitution;
        mol.force.y *= -mol.restitution;
    }
    else if (mol.position.y - mol.radius < CONTAINER_Y)
    {
        mol.position.y = CONTAINER_Y + mol.radius;
        mol.velocity.y *= -mol.restitution;
        mol.force.y *= -mol.restitution;
    }

    if (chamber.checkSensor(mol.position, mol.radius))
    {
        mol.isCounted = true;
        if (mol.position.x > static_cast<float>(screenWidth)/2.0f)
        {
            mol.isLeft = false;
        }
        else
        {
            mol.isLeft = true;
        }
    }
}

void Gas::CheckCollision(Molecule &mol)
{
    Cell zone = grid.getZone(mol.position, mol.radius, mol.id);

    for (auto cid : zone)
    {
        if (cid == mol.id)
        {
            continue;
        }

        Molecule* other = &molecules[cid];

        if (CheckCollisionCircles(other->position, other->radius, mol.position, mol.radius))
        {
            Repulse(mol, other);
            other->collided = true;
            mol.collided = true;
        }
    }
}

void Gas::CollideZone(Molecule &mol)
{
    if (!mol.collided)
        return;

    Cell zone = grid.getZone(mol.position, mol.radius, mol.id);

    for (auto cid : zone)
    {
        Molecule* other = &molecules[cid];

        if (cid == mol.id || !other->collided)
        {
            continue;
        }

        Collide(mol, other);
    }
}

void Gas::Collide(Molecule &m1, Molecule *m2)
{
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

    m1.collided = false;
    m2->collided = false;
}

void Gas::Repulse(Molecule &m1, Molecule *m2)
{
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

void Gas::UpdateMovement(Molecule &mol, Vector2 force)
{
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
    if (mol.velocity.x > MAX_SPEED)
    {
        mol.velocity.x = MAX_SPEED;
    }

    if (mol.velocity.x < -MAX_SPEED)
    {
        mol.velocity.x = -MAX_SPEED;
    }

    if (mol.velocity.y > MAX_SPEED)
    {
        mol.velocity.y = MAX_SPEED;
    }

    if (mol.velocity.y < -MAX_SPEED)
    {
        mol.velocity.y = -MAX_SPEED;
    }

    // color update
    Vector2 normalVel = Vector2Normalize(newVelocity);
    mol.color = ColorLerp(mol.color1, mol.color2, EASE_OUT_EXPO((normalVel.x + normalVel.y) / 10.0f));
}


// float Locate::Top(const Molecule& m) { return m.position.y - m.radius; }
// float Locate::Top(Vector2 position, float radius) { return position.y - radius; }

// float Locate::Left(const Molecule& m) { return m.position.x - m.radius; }
// float Locate::Left(Vector2 position, float radius) { return position.x - radius; }

// float Locate::Right(const Molecule& m) { return m.position.x + m.radius; }
// float Locate::Right(Vector2 position, float radius) { return position.x + radius; }

// float Locate::Bottom(const Molecule& m) { return m.position.y + m.radius; }
// float Locate::Bottom(Vector2 position, float radius) { return position.y + radius; }
