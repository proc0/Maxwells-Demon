#include "gas.hpp"
#include "raylib.h"

void Gas::Load(const Chamber& chamber)
{
    containerLeft = Rectangle({ CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    containerRight = Rectangle({ CONTAINER_X + CONTAINER_WIDTH/2.0f, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    colorChamberLeft = BLUE;
    colorChamberRight = RED;
    barColor = RED;
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS * 4);
    Populate(chamber);
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

void Gas::Populate(const Chamber& chamber)
{
    // grid.addWalls(chamber.walls);
    grid.addWall(chamber.walls[0]);
    grid.addWall(chamber.walls[1]);
    grid.addWall(chamber.walls[2]);

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

    float halfHotCount = totalHotCount/2.0f;
    float halfColdCount = totalCoolCount/2.0f;
    maxEntropy = calculateBoltzmannEntropy(halfHotCount, totalHotCount - halfHotCount, halfColdCount, totalCoolCount - halfColdCount);
    entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    completion = entropy / maxEntropy;
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

    DrawRectangleRec(containerLeft, colorChamberLeft);
    DrawRectangleRec(containerRight, colorChamberRight);

    // grid.Render();

    for (const Molecule &mol : molecules)
    {
        if (!mol.active)
            continue;

        DrawCircle(mol.position.x, mol.position.y, mol.radius, mol.color);
    }

    const char *leftHotCountText = TextFormat("%.f", leftChamberHotCount);
    const char *leftColdCountText = TextFormat("%.f", leftChamberCoolCount);
    const char *rightHotCountText = TextFormat("%.f", rightChamberHotCount);
    const char *rightColdCountText = TextFormat("%.f", rightChamberCoolCount);

    DrawText(leftHotCountText, 280, 15, 20, RED);
    DrawText(pipeText, 300, 15, 20, BLACK); 
    DrawText(leftColdCountText, 308, 15, 20, BLUE);

    DrawText(rightHotCountText, 948, 15, 20, RED);
    DrawText(pipeText, 970, 15, 20, BLACK);
    DrawText(rightColdCountText, 980, 15, 20, BLUE);

    const char *entText = TextFormat("Entropy %.2f", entropy);
    const char *maxEntText = TextFormat("Max %.2f", maxEntropy);
    DrawText(entText, 430, 15, 20, BLACK);
    DrawText(maxEntText, 730, 15, 20, BLACK);

    DrawRectangle(entropyBarX-1, entropyBarY-1, entropyBarLength+2, 7, BLACK);
    DrawRectangle(entropyBarX, entropyBarY, entropyBarLength, 5, RAYWHITE);
    DrawRectangle(entropyBarX, entropyBarY, entropyBar, 5, barColor);

}

void Gas::Update(const Chamber& chamber)
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

    // grid.updateWalls();

    entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    completion = entropy / maxEntropy;
    entropyBar = entropyBarLength * completion;
    barColor = ColorLerp(GREEN, GRAY, completion);

    if(leftChamberCoolCount == leftChamberHotCount) {
        colorChamberLeft = RAYWHITE;
    } else {
        colorChamberLeft = Fade(leftChamberCoolCount > leftChamberHotCount ? ColorLerp(RAYWHITE, BLUE, leftChamberCoolCount/totalCoolCount) : ColorLerp(RAYWHITE, RED, leftChamberHotCount/totalHotCount), 0.2f);
    }

    if(rightChamberCoolCount == rightChamberHotCount) {
        colorChamberRight = RAYWHITE;
    } else {
        colorChamberRight = Fade(rightChamberCoolCount > rightChamberHotCount ? ColorLerp(RAYWHITE, BLUE, rightChamberCoolCount/totalCoolCount) : ColorLerp(RAYWHITE, RED, rightChamberHotCount/totalHotCount), 0.2f);
    }

}

void Gas::Unload()
{
}

void Gas::CheckBounds(Molecule &mol, const Chamber& chamber)
{

    Cell wallZone = grid.getWalls(&mol);

    for (auto cid : wallZone)
    {
        Wall2 wall = chamber.walls[cid];
        Rectangle wallRect = wall.rect;
        if (CheckCollisionCircleRec(mol.position, mol.radius, wallRect))
        {
            if (Locate::Right(mol) > wallRect.x && mol.position.x < wallRect.x && mol.velocity.x > 0)
            {
                mol.position.x = wallRect.x - mol.radius - 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            }
            else if (Locate::Left(mol) < wallRect.x + wallRect.width && mol.position.x > wallRect.x && mol.velocity.x < 0)
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

float Gas::calculateShannonEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const
{
    // Stirling Approx. : n*(N!) ≈ N*lnN − N
    // This has negative results (error approx.) for small numbers
    float totalCountHot = leftHotCount + rightHotCount;
    float totalCountCold = leftColdCount + rightColdCount;
    float p = leftHotCount / totalCountHot;
    float q = rightHotCount / totalCountHot;
    float r = leftColdCount / totalCountCold;
    float t = rightColdCount / totalCountCold;

    float leftHotMember = p != 1 && p != 0 ? totalCountHot * (p * log(p) + (1 - p) * log(1 - p)) : 0;
    float leftCoolMember = r != 1 && r != 0 ? totalCountCold * (r * log(r) + (1 - r) * log(1 - r)) : 0;
    float leftEntropy = leftHotMember == 0 ? leftCoolMember : -leftHotMember - leftCoolMember;

    float rightHotMember = q != 1 && q != 0 ? totalCountHot * (q * log(q) + (1 - q) * log(1 - q)) : 0;
    float rightCoolMember = t != 1 && t != 0 ? totalCountCold * (t * log(t) + (1 - t) * log(1 - t)) : 0;
    float rightEntropy = rightHotMember == 0 ? rightCoolMember : -rightHotMember - rightCoolMember;

    return leftEntropy + rightEntropy;
}

float factorial(const int n)
{
    float f = 1;
    for (int i = 1; i <= n; ++i)
        f *= i;
    return f;
}

float Gas::calculateBoltzmannEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const
{   // S = k*ln*W
    // W = N1! / N2!*N3!
    // where N1 is total count of molecules, N2, N3 are chamber counts
    float totalCountHot = leftHotCount + rightHotCount;
    float totalCountCold = leftColdCount + rightColdCount;
    float hotEntropy = factorial(totalCountHot) / (factorial(leftHotCount) * factorial(rightHotCount));
    float coldEntropy = factorial(totalCountCold) / (factorial(leftColdCount) * factorial(rightColdCount));

    return log(hotEntropy * coldEntropy);
}

// GRID

void Grid::Load(short width, short height, float unit)
{
    cellSize = unit;
    short columns = static_cast<short>(ceil(static_cast<float>(width) / cellSize));
    short rows = static_cast<short>(ceil(static_cast<float>(height) / cellSize));

    cellCount = Vector2(columns, rows);
    cells.resize(columns, std::vector<Cell>(rows));
    wallCells.resize(columns, std::vector<Cell>(rows));

    for (short x = 0; x < columns; ++x)
    {
        for (short y = 0; y < rows; ++y)
        {
            cells[x][y] = Cell();
            wallCells[x][y] = Cell();
        }
    }
}

Vector2 Grid::place(float _x, float _y) const
{
    int x = floor(static_cast<int>((_x - CONTAINER_X) / cellSize));
    int y = floor(static_cast<int>((_y - CONTAINER_Y) / cellSize));

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x >= cellCount.x)
        x = cellCount.x - 1;
    if (y >= cellCount.y)
        y = cellCount.y - 1;

    return Vector2(x, y);
}

void Grid::addPoint(Vector2 point, short id)
{
    Vector2 cell = place(point.x, point.y);
    cells[cell.x][cell.y].push_back(id);
    // mol->cell = cell;
}

void Grid::addArea(Rectangle area, short id)
{
    Vector2 cell = place(area.x, area.y);

    int widthCells = ceil(area.width / cellSize);
    int heightCells = ceil(area.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++)
    {
        for (int y = cell.y; y < cell.y + heightCells; y++)
        {
            wallCells[x][y].push_back(id);
            // wall->cells.push_back(Vector2(x, y));
        }
    }
}

void Grid::removeArea(Rectangle area, short id)
{
    Vector2 position = place(area.x, area.y);

    int widthCells = ceil(area.width / cellSize);
    int heightCells = ceil(area.height / cellSize);

    for (int x = position.x; x < position.x + widthCells; x++)
    {
        for (int y = position.y; y < position.y + heightCells; y++)
        {
            auto &cell = wallCells[x][y];

            auto newEnd = std::remove(cell.begin(), cell.end(), id);
            cell.erase(newEnd, cell.end());
        }
    }
}

Cell Grid::getArea(Vector2 point, float radius)
{
    Vector2 topLeft = place(Locate::Left(point, radius), Locate::Top(point, radius));
    Vector2 bottomRight = place(Locate::Right(point, radius), Locate::Bottom(point, radius));

    Cell wallZone;
    std::vector<short> processed;
    for (short x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (short y = topLeft.y; y <= bottomRight.y; ++y)
        {
            auto cell = wallCells[x][y];
            for (auto cid : cell)
            {
                bool isProcessed = false;
                for (short procId : processed)
                {
                    if (procId == cid)
                    {
                        isProcessed = true;
                        break;
                    }
                }
                if (!isProcessed)
                {
                    wallZone.push_back(cid);
                    processed.push_back(cid);
                }
            }
        }
    }

    return wallZone;
}


// void Grid::addWalls(std::vector<Rectangle> &wallRects)
// {
//     for (int i = 0; i < (int)wallRects.size(); i++)
//     {
//         Wall wall = {
//             .rect = wallRects.at(i),
//             // .cells = std::vector<Vector2>(),
//             .id = i,
//         };
//         walls.push_back(wall.id);
//         // addWall(&walls.back());
//     }

//     for (auto &wall : walls)
//     {
//         addWall(&wall);
//     }
// }

void Grid::addWall(const Wall2& wall)
{
    Vector2 cell = place(wall.rect.x, wall.rect.y);

    int widthCells = ceil(wall.rect.width / cellSize);
    int heightCells = ceil(wall.rect.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++)
    {
        for (int y = cell.y; y < cell.y + heightCells; y++)
        {
            wallCells[x][y].push_back(wall.id);
            // wall->cells.push_back(Vector2(x, y));
        }
    }
}

void Grid::removeWall(const Wall2& wall)
{
    Vector2 cell = place(wall.rect.x, wall.rect.y);

    int widthCells = ceil(wall.rect.width / cellSize);
    int heightCells = ceil(wall.rect.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++)
    {
        for (int y = cell.y; y < cell.y + heightCells; y++)
        {
            auto& wallCell = wallCells[x][y];

            auto newEnd = std::remove(wallCell.begin(), wallCell.end(), wall.id);
            wallCell.erase(newEnd, wallCell.end());
        }
    }
}

void Grid::remove(Vector2 cell, short id)
{
    auto gridCell = cells[cell.x][cell.y];

    auto newEnd = std::remove(gridCell.begin(), gridCell.end(), id);
    gridCell.erase(newEnd, gridCell.end());
}

// void Grid::Render() const
// {
//     // for (int x = 0; x < cellCount.x; ++x) {
//     //     for (int y = 0; y < cellCount.y; ++y) {
//     //         DrawPixel(x*cellSize + CONTAINER_X, y*cellSize + CONTAINER_Y, RED);
//     //     }
//     // }
//     // DrawRectangleRec(sensor, RED);

//     for (const Wall &wall : walls)
//     {
//         DrawRectangleRec(wall.rect, BLACK);
//     }
// }

Vector2 Grid::update(Vector2 point, Vector2 cell, short id)
{
    Vector2 newCell = place(point.x, point.y);
    if (newCell.x != cell.x || newCell.y != cell.y)
    {
        remove(cell, id);
        cells[newCell.x][newCell.y].push_back(id);
        return newCell;
    }

    return Vector2(0, 0);
}

// void Grid::updateWalls()
// {
//     Wall *wall = &walls[1];

//     if (IsKeyReleased(KEY_SPACE))
//     {
//         isDoorClosing = true;
//     }

//     if (isDoorClosing)
//     {

//         if (doorFrame == 0 && wall->rect.y < DOOR_MAX_Y)
//         {
//             doorFrame = DOOR_OPEN_FRAMES;
//         }

//         if (doorFrame > 0)
//         {
//             float displacement = EASE_OUT_EXPO((1 / (float)doorFrame) * DOOR_OPEN_FRAMES);
//             wall->rect.y += displacement * 10;
//             if (wall->rect.y >= DOOR_MAX_Y)
//             {
//                 wall->rect.y = DOOR_MAX_Y;
//                 isDoorClosing = false;
//             }
//             // addWall(wall);
//         }
//     }

//     if (IsKeyDown(KEY_SPACE))
//     {
//         isDoorClosing = false;

//         if (doorFrame == 0 && wall->rect.y > DOOR_MIN_Y)
//         {
//             doorFrame = DOOR_OPEN_FRAMES;
//         }

//         if (doorFrame > 0)
//         {
//             float displacement = EASE_OUT_EXPO((1 / (float)doorFrame) * DOOR_OPEN_FRAMES);
//             wall->rect.y -= displacement * 10;
//             if (wall->rect.y <= DOOR_MIN_Y)
//             {
//                 wall->rect.y = DOOR_MIN_Y;
//             }
//             // removeWall(wall);

//             doorFrame--;
//         }
//     }
// }

Cell Grid::getZone(Vector2 point, float radius, short id)
{
    Vector2 topLeft = place(Locate::Left(point, radius), Locate::Top(point, radius));
    Vector2 bottomRight = place(Locate::Right(point, radius), Locate::Bottom(point, radius));

    Cell zone;

    for (int x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y <= bottomRight.y; ++y)
        {
            Cell cell = cells[x][y];
            for (auto cid : cell)
            {
                if (id != cid)
                {
                    zone.push_back(cid);
                }
            }
        }
    }

    return zone;
}

Cell Grid::getWalls(Molecule *mol)
{
    Vector2 topLeft = place(Locate::Left(*mol), Locate::Top(*mol));
    Vector2 bottomRight = place(Locate::Right(*mol), Locate::Bottom(*mol));

    Cell wallZone;
    std::vector<short> processed;
    for (short x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (short y = topLeft.y; y <= bottomRight.y; ++y)
        {
            const auto &wallCell = wallCells[x][y];
            for (auto cid : wallCell)
            {
                bool isProcessed = false;
                for (short procId : processed)
                {
                    if (procId == cid)
                    {
                        isProcessed = true;
                        break;
                    }
                }
                if (!isProcessed)
                {
                    wallZone.push_back(cid);
                    processed.push_back(cid);
                }
            }
        }
    }

    return wallZone;
}

// bool Grid::checkTunneling(Vector2 position, float radius)
// {
//     bool isIntersect = false;
//     for (auto &wall : walls)
//     {
//         if (CheckCollisionCircleRec(position, radius, wall.rect))
//         {
//             isIntersect = true;
//             break;
//         }
//     }

//     return isIntersect;
// };

// bool Grid::checkSensor(Molecule &mol)
// {
//     Vector2 sensorCell = place(sensor.x, sensor.y);

//     int widthCells = ceil(sensor.width / cellSize);
//     int heightCells = ceil(sensor.height / cellSize);

//     bool isDetected = false;
//     for (int x = sensorCell.x; x < sensorCell.x + widthCells; x++)
//     {
//         for (int y = sensorCell.y; y < sensorCell.y + heightCells; y++)
//         {
//             isDetected = CheckCollisionCircleRec(mol.position, mol.radius, sensor);
//         }
//     }

//     return isDetected;
// }

float Locate::Top(const Molecule& m) { return m.position.y - m.radius; }
float Locate::Top(Vector2 position, float radius) { return position.y - radius; }

float Locate::Left(const Molecule& m) { return m.position.x - m.radius; }
float Locate::Left(Vector2 position, float radius) { return position.x - radius; }

float Locate::Right(const Molecule& m) { return m.position.x + m.radius; }
float Locate::Right(Vector2 position, float radius) { return position.x + radius; }

float Locate::Bottom(const Molecule& m) { return m.position.y + m.radius; }
float Locate::Bottom(Vector2 position, float radius) { return position.y + radius; }
