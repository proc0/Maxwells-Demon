#include "gas.hpp"

void Gas::Load()
{
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS * 4);
    Populate();
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

    grid.add(&molecules[0]);

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

    grid.add(&molecules[1]);
}

void Gas::Populate()
{
    grid.addWalls(wallRects);

    int maxHotMoleculates = float(DENSITY) / 2;
    for (int i = 0; i < DENSITY; i++)
    {
        if (molecules[i].active)
        {
            continue;
        }

        Vector2 vel = {float(GetRandomValue(-100, 100)), float(GetRandomValue(-100, 100))};
        bool isHot = totalHotCount < maxHotMoleculates ? GetRandomValue(0, 1) == 1 : false;

        Color color1 = isHot ? BEIGE : DARKBLUE;
        Color color2 = isHot ? RED : BLUE;

        float randNegX = float(GetRandomValue(-250, -150));
        float randNegY = float(GetRandomValue(-250, -150));
        float randPosX = float(GetRandomValue(150, 20));
        float randPosY = float(GetRandomValue(150, 250));
        Vector2 randForce = Vector2(GetRandomValue(0, 1) == 1 ? randNegX : randPosX, GetRandomValue(0, 1) == 1 ? randNegY : randPosY);
        molecules[i] = {
            // .force = {0, 0},
            .force = isHot ? randForce : ZERO_FORCE,
            // .force = { GetRandomValue(0, 1) == 1 ? float(GetRandomValue(-500, 0)) : float(GetRandomValue(500, 0)), GRAVITY },
            .origin = {0.0f, 0.0f},
            .position = Spawn(MOLECULE_RADIUS),
            .velocity = {0, 0},
            // .velocity = vel,
            .acceleration = {0.0f, 0.0f},
            .color1 = color1,
            .color2 = color2,
            .color = ColorLerp(color1, color2, fabsf(vel.x) + fabsf(vel.y)),
            .mass = 1.0f,
            .radius = MOLECULE_RADIUS,
            .restitution = isHot ? 1.0f : RESTITUTION,
            .id = i,
            .active = true,
            .collided = false,
            .isHot = isHot};

        grid.add(&molecules[i]);

        if (molecules[i].isHot)
        {
            totalHotCount++;
            if (molecules[i].position.x > GetScreenWidth() / 2)
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
            if (molecules[i].position.x > GetScreenWidth() / 2)
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

    float halfHotCount = float(totalHotCount) / 2;
    float halfColdCount = float(totalCoolCount) / 2;
    maxEntropy = calculateBoltzmannEntropy(halfHotCount, totalHotCount - halfHotCount, halfColdCount, totalCoolCount - halfColdCount);
    entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    completion = entropy / maxEntropy;
}

Vector2 Gas::Spawn(float radius)
{
    Vector2 position = {float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH)), float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT))};
    int retries = 12;

    while (retries >= 0)
    {
        bool isIntersect = grid.checkTunneling(position, radius);
        if (!isIntersect)
            break;

        position = {float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH)), float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT))};
        retries--;
    }

    if (grid.checkTunneling(position, radius))
    {
        position = {CONTAINER_X + 10, CONTAINER_Y + 10};
    }

    return position;
}

void Gas::Render() const
{

    DrawRectangle(CONTAINER_X - 15, CONTAINER_Y - 15, CONTAINER_WIDTH + 30, CONTAINER_HEIGHT + 30, BLACK);
    DrawRectangle(CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH, CONTAINER_HEIGHT, RAYWHITE);

    // DrawRectangleRec(wallRect, BLACK);
    grid.Render();

    for (const Molecule &mol : molecules)
    {
        if (!mol.active)
            continue;

        DrawCircle(mol.position.x, mol.position.y, mol.radius, mol.color);
    }

    const char *leftCount = TextFormat("Left: %d/%d", leftChamberCoolCount, leftChamberHotCount);
    const char *rightCount = TextFormat("Right: %d/%d", rightChamberCoolCount, rightChamberHotCount);
    const char *entText = TextFormat("Entropy: %f", entropy);
    const char *maxEntText = TextFormat("Max Entropy: %f", maxEntropy);
    DrawText(leftCount, 50, 10, 20, BLACK);
    DrawText(rightCount, 850, 10, 20, BLACK);
    DrawText(entText, 630, 10, 20, BLACK);
    DrawText(maxEntText, 330, 10, 20, BLACK);

    Color barColor = completion < 0.5 ? GREEN : RED;
    DrawRectangle(330, 45, 500 * completion, 15, barColor);
}

void Gas::Update()
{
    // if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
    //     for (Molecule& mol : molecules) {
    //         mol.position = { x: float(GetRandomValue(CONTAINER_X, CONTAINER_X + CONTAINER_WIDTH-3)), y: float(GetRandomValue(CONTAINER_Y, CONTAINER_Y + CONTAINER_HEIGHT-3)) };
    //     }
    // }
    for (Molecule &mol : molecules)
    {
        CheckBounds(mol);
        UpdateMovement(mol, ZERO_FORCE);
        CollideZone(mol);
        grid.update(&mol);
    }

    int _rightChamberHotCount = 0;
    int _rightChamberCoolCount = 0;
    int _leftChamberHotCount = 0;
    int _leftChamberCoolCount = 0;
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

    grid.updateWalls();

    entropy = calculateBoltzmannEntropy(leftChamberHotCount, rightChamberHotCount, leftChamberCoolCount, rightChamberCoolCount);
    completion = entropy / maxEntropy;
}

void Gas::Unload()
{
}

void Gas::CheckBounds(Molecule &mol)
{

    std::vector<Wall *> wallZone = grid.getWalls(&mol);

    for (auto &wall : wallZone)
    {
        Rectangle wallRect = wall->rect;
        if (CheckCollisionCircleRec(mol.position, mol.radius, wallRect))
        {
            if (mol.getRight() > wallRect.x && mol.position.x < wallRect.x && mol.velocity.x > 0)
            {
                mol.position.x = wallRect.x - mol.radius - 2;
                mol.velocity.x *= -mol.restitution;
                mol.force.x *= -mol.restitution;
            }
            else if (mol.getLeft() < wallRect.x + wallRect.width && mol.position.x > wallRect.x && mol.velocity.x < 0)
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

    if (grid.checkSensor(mol))
    {
        mol.isCounted = true;
        if (mol.position.x > GetScreenWidth() / 2)
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
    std::vector<Molecule *> zone = grid.getZone(&mol);

    for (Molecule *other : zone)
    {
        if (other->id == mol.id)
        {
            continue;
        }

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

    std::vector<Molecule *> zone = grid.getZone(&mol);

    for (Molecule *other : zone)
    {
        if (other->id == mol.id || !other->collided)
        {
            continue;
        }

        Collide(mol, other);
    }
}

// TODO: add inelastic collision by factoring in RESTITUION
// update the color of the ball from red to blue as it loses energy
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
    float normalVelocity1 = (m1.restitution * normalComponent1 * (m1.mass - m2->mass) + 2 * m2->mass * normalComponent2) / totalMass;
    float normalVelocity2 = (m2->restitution * normalComponent2 * (m2->mass - m1.mass) + 2 * m1.mass * normalComponent1) / totalMass;

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

    Vector2 normalVel = Vector2Normalize(newVelocity);
    mol.color = ColorLerp(mol.color1, mol.color2, EASE_OUT_EXPO((normalVel.x + normalVel.y) / 10));
    // mol.color = ColorLerp(mol.color1, mol.color2, Vector2Length(normalVel)/2);
}

float Gas::calculateShannonEntropy() const
{
    // Stirling Approx. : n*(N!) ≈ N*lnN − N
    // This has negative results (error approx.) for small numbers
    float p = float(leftChamberHotCount) / float(totalHotCount);
    float q = float(rightChamberHotCount) / float(totalHotCount);
    float r = float(leftChamberCoolCount) / float(totalCoolCount);
    float t = float(rightChamberCoolCount) / float(totalCoolCount);

    float leftHotMember = p != 1 && p != 0 ? float(totalHotCount) * (p * log(p) + (1 - p) * log(1 - p)) : 0;
    float leftCoolMember = r != 1 && r != 0 ? float(totalCoolCount) * (r * log(r) + (1 - r) * log(1 - r)) : 0;
    float leftEntropy = leftHotMember == 0 ? leftCoolMember : -leftHotMember - leftCoolMember;

    float rightHotMember = q != 1 && q != 0 ? float(totalHotCount) * (q * log(q) + (1 - q) * log(1 - q)) : 0;
    float rightCoolMember = t != 1 && t != 0 ? float(totalCoolCount) * (t * log(t) + (1 - t) * log(1 - t)) : 0;
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

float Gas::calculateBoltzmannEntropy(int leftHotCount, int rightHotCount, int leftColdCount, int rightColdCount) const
{
    float totalCountHot = leftHotCount + rightHotCount;
    float totalCountCold = leftColdCount + rightColdCount;
    float hotEntropy = factorial(float(totalCountHot)) / (factorial(float(leftHotCount)) * factorial(float(rightHotCount)));
    float coldEntropy = factorial(float(totalCountCold)) / (factorial(float(leftColdCount)) * factorial(float(rightColdCount)));

    return log(hotEntropy * coldEntropy);
}

void Grid::Load(int gridWidth, int gridHeight, float _cellSize)
{
    cellSize = _cellSize;
    int columns = static_cast<int>(std::ceil(static_cast<float>(gridWidth) / cellSize));
    int rows = static_cast<int>(std::ceil(static_cast<float>(gridHeight) / cellSize));

    cellCount = Vector2(columns, rows);
    cells.resize(columns, std::vector<std::vector<Molecule *>>(rows));
    wallCells.resize(columns, std::vector<std::vector<Wall *>>(rows));

    for (int x = 0; x < columns; ++x)
    {
        for (int y = 0; y < rows; ++y)
        {
            cells[x][y] = std::vector<Molecule *>();
            wallCells[x][y] = std::vector<Wall *>();
        }
    }
}

Vector2 Grid::place(float _x, float _y) const
{
    int x = std::floor(static_cast<int>((_x - CONTAINER_X) / cellSize));
    int y = std::floor(static_cast<int>((_y - CONTAINER_Y) / cellSize));

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

void Grid::add(Molecule *mol)
{
    Vector2 cell = place(mol->position.x, mol->position.y);
    cells[cell.x][cell.y].push_back(mol);
    mol->cell = cell;
}

void Grid::addWalls(std::vector<Rectangle> &wallRects)
{
    for (int i = 0; i < (int)wallRects.size(); i++)
    {
        Wall wall = {
            .rect = wallRects.at(i),
            // .cells = std::vector<Vector2>(),
            .id = i,
        };
        walls.push_back(wall);
        // addWall(&walls.back());
    }

    for (auto &wall : walls)
    {
        addWall(&wall);
    }
}

void Grid::addWall(Wall *wall)
{
    Vector2 cell = place(wall->rect.x, wall->rect.y);

    int widthCells = std::ceil(wall->rect.width / cellSize);
    int heightCells = std::ceil(wall->rect.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++)
    {
        for (int y = cell.y; y < cell.y + heightCells; y++)
        {
            wallCells[x][y].push_back(wall);
            wall->cells.push_back(Vector2(x, y));
        }
    }
}

void Grid::removeWall(Wall *wall)
{
    Vector2 cell = place(wall->rect.x, wall->rect.y);

    int widthCells = std::ceil(wall->rect.width / cellSize);
    int heightCells = std::ceil(wall->rect.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++)
    {
        for (int y = cell.y; y < cell.y + heightCells; y++)
        {
            auto &cellWalls = wallCells[x][y];

            auto newEnd = std::remove(cellWalls.begin(), cellWalls.end(), wall);
            cellWalls.erase(newEnd, cellWalls.end());
        }
    }
}

void Grid::remove(Molecule *mol)
{
    const Vector2 &cell = mol->cell;
    auto &cellMolecules = cells[cell.x][cell.y];

    auto newEnd = std::remove(cellMolecules.begin(), cellMolecules.end(), mol);
    cellMolecules.erase(newEnd, cellMolecules.end());
}

void Grid::Render() const
{
    // for (int x = 0; x < cellCount.x; ++x) {
    //     for (int y = 0; y < cellCount.y; ++y) {
    //         DrawPixel(x*cellSize + CONTAINER_X, y*cellSize + CONTAINER_Y, RED);
    //     }
    // }
    // DrawRectangleRec(sensor, RED);

    for (const Wall &wall : walls)
    {
        DrawRectangleRec(wall.rect, BLACK);
    }
}

void Grid::update(Molecule *mol)
{
    Vector2 newCell = place(mol->position.x, mol->position.y);
    if (newCell.x != mol->cell.x || newCell.y != mol->cell.y)
    {
        remove(mol);
        mol->cell = newCell;
        cells[newCell.x][newCell.y].push_back(mol);
    }
}

void Grid::updateWall(Wall *wall)
{
    removeWall(wall);
    addWall(wall);
}

void Grid::updateWalls()
{
    Wall *wall = &walls[1];

    if (IsKeyReleased(KEY_SPACE))
    {
        isDoorClosing = true;
    }

    if (isDoorClosing)
    {

        if (doorFrame == 0 && wall->rect.y < DOOR_MAX_Y)
        {
            doorFrame = DOOR_OPEN_FRAMES;
        }

        if (doorFrame > 0)
        {
            float displacement = EASE_OUT_EXPO((1 / (float)doorFrame) * DOOR_OPEN_FRAMES);
            wall->rect.y += displacement * 10;
            if (wall->rect.y >= DOOR_MAX_Y)
            {
                wall->rect.y = DOOR_MAX_Y;
                isDoorClosing = false;
            }
            // addWall(wall);
        }
    }

    if (IsKeyDown(KEY_SPACE))
    {
        isDoorClosing = false;

        if (doorFrame == 0 && wall->rect.y > DOOR_MIN_Y)
        {
            doorFrame = DOOR_OPEN_FRAMES;
        }

        if (doorFrame > 0)
        {
            float displacement = EASE_OUT_EXPO((1 / (float)doorFrame) * DOOR_OPEN_FRAMES);
            wall->rect.y -= displacement * 10;
            if (wall->rect.y <= DOOR_MIN_Y)
            {
                wall->rect.y = DOOR_MIN_Y;
            }
            // removeWall(wall);

            doorFrame--;
        }
    }
}

std::vector<Molecule *> Grid::getZone(Molecule *mol)
{
    Vector2 topLeft = place(mol->getLeft(), mol->getTop());
    Vector2 bottomRight = place(mol->getRight(), mol->getBottom());

    int queryId = ++queryIds;
    std::vector<Molecule *> zone;

    for (int x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y <= bottomRight.y; ++y)
        {
            const auto &cellMolecules = cells[x][y];
            for (Molecule *m : cellMolecules)
            {
                if (m->id != mol->id && m->queryId != queryId)
                {
                    m->queryId = queryId;
                    zone.push_back(m);
                }
            }
        }
    }

    return zone;
}

std::vector<Wall *> Grid::getWalls(Molecule *mol)
{
    Vector2 topLeft = place(mol->getLeft(), mol->getTop());
    Vector2 bottomRight = place(mol->getRight(), mol->getBottom());

    std::vector<Wall *> wallZone;
    std::vector<int> processed;
    for (int x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y <= bottomRight.y; ++y)
        {
            const auto &wallBounds = wallCells[x][y];
            for (auto &wall : wallBounds)
            {
                bool isProcessed = false;
                for (int procId : processed)
                {
                    if (procId == wall->id)
                    {
                        isProcessed = true;
                        break;
                    }
                }
                if (!isProcessed)
                {
                    wallZone.push_back(wall);
                    processed.push_back(wall->id);
                }
            }
        }
    }

    return wallZone;
}

bool Grid::checkTunneling(Vector2 position, float radius)
{
    bool isIntersect = false;
    for (auto &wall : walls)
    {
        if (CheckCollisionCircleRec(position, radius, wall.rect))
        {
            isIntersect = true;
            break;
        }
    }

    return isIntersect;
};

bool Grid::checkSensor(Molecule &mol)
{
    // Vector2 molCell = place(mol.position.x, mol.position.y);
    // Vector2 cellsCenter = place(640, 450); // sensor rect center
    Vector2 sensorCell = place(sensor.x, sensor.y);

    int widthCells = std::ceil(sensor.width / cellSize);
    int heightCells = std::ceil(sensor.height / cellSize);

    bool isDetected = false;
    for (int x = sensorCell.x; x < sensorCell.x + widthCells; x++)
    {
        for (int y = sensorCell.y; y < sensorCell.y + heightCells; y++)
        {
            isDetected = CheckCollisionCircleRec(mol.position, mol.radius, sensor);
        }
    }

    return isDetected;
}

void Grid::clear()
{
    for (auto &col : cells)
    {
        for (auto &cell : col)
        {
            cell.clear();
        }
    }
}
