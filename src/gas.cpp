#include "gas.hpp"

void Gas::Load() {
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS);

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
            .radius = MOLECULE_RADIUS,
            .id = i,
            .active = true,
            .collided = false,
            .debounce = 60,
        };

        grid.add(&molecules[i]);
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
        grid.update(&mol);
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
    }

    if(mol.position.y + mol.radius > CONTAINER_HEIGHT + CONTAINER_Y - 3) {
        mol.position.y = CONTAINER_HEIGHT + CONTAINER_Y - 3 - mol.radius;
        mol.velocity.y *= -RESTITUTION;
    } else if(mol.position.y - mol.radius < CONTAINER_Y + 3) {
        mol.position.y = CONTAINER_Y + 3 + mol.radius;
        mol.velocity.y *= -RESTITUTION;
    }
}

void Gas::CheckCollision(Molecule &mol) {
    std::vector<Molecule*> zone = grid.getZone(&mol);

    for(Molecule* other: zone) {
        if(other->id == mol.id){
            continue;
        }

        if(CheckCollisionCircles(other->position, other->radius, mol.position, mol.radius)){
            Repulse(mol, *other);
            other->collided = true;
            mol.collided = true;
        }
    }
}

void Gas::CollideZone(Molecule &mol) {
    if(!mol.collided) return;

    std::vector<Molecule*> zone = grid.getZone(&mol);
    
    for(Molecule* other: zone) {
        if(other->id == mol.id || !other->collided){
            continue;
        }

        Collide(mol, *other);

        mol.collided = false;
        other->collided = false;
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
}

void Grid::Load(int gridWidth, int gridHeight, float _cellSize) {
    cellSize = _cellSize;
    int columns = static_cast<int>(std::ceil(static_cast<float>(gridWidth) / cellSize));
    int rows = static_cast<int>(std::ceil(static_cast<float>(gridHeight) / cellSize));

    cellCount = Vector2(columns, rows);
    cells.resize(columns, std::vector<std::vector<Molecule*>>(rows));

    for (int x = 0; x < columns; ++x) {
        for (int y = 0; y < rows; ++y) {
            cells[x][y] = std::vector<Molecule*>();
        }
    }
}

Vector2 Grid::place(const Molecule* mol) const {
    int x = static_cast<int>(mol->position.x / cellSize);
    int y = static_cast<int>(mol->position.y / cellSize);

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= cellCount.x) x = cellCount.x - 1;
    if (y >= cellCount.y) y = cellCount.y - 1;

    return Vector2(x, y);
}

void Grid::add(Molecule* mol) {
    Vector2 cell = place(mol);
    cells[cell.x][cell.y].push_back(mol);
    mol->cell = cell;
}

void Grid::remove(Molecule* mol) {
    const Vector2& cell = mol->cell;
    auto& cellParticles = cells[cell.x][cell.y];

    auto it = std::find(cellParticles.begin(), cellParticles.end(), mol);
    if (it != cellParticles.end()) {
        *it = cellParticles.back();
        cellParticles.pop_back();
    }
}

void Grid::update(Molecule* mol) {
    Vector2 newCell = place(mol);
    if (newCell.x != mol->cell.x || newCell.y != mol->cell.y) {
        remove(mol);
        mol->cell = newCell;
        cells[newCell.x][newCell.y].push_back(mol);
    }
}

std::vector<Molecule*> Grid::getZone(Molecule* mol) {
    int left   = static_cast<int>(std::floor(mol->getLeft()   / cellSize));
    int right  = static_cast<int>(std::floor(mol->getRight()  / cellSize));
    int top    = static_cast<int>(std::floor(mol->getTop()    / cellSize));
    int bottom = static_cast<int>(std::floor(mol->getBottom() / cellSize));

    int queryId = ++queryIds;
    std::vector<Molecule*> zone;

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            if (x < 0 || y < 0 || x >= cellCount.x || y >= cellCount.y)
                continue;

            const auto& cellParticles = cells[x][y];
            for (Molecule* p : cellParticles) {
                if (p != mol && p->queryId != queryId) {
                    p->queryId = queryId;
                    zone.push_back(p);
                }
            }
        }
    }

    return zone;
}

void Grid::clear() {
    for (auto& col : cells) {
        for (auto& cell : col) {
            cell.clear();
        }
    }
}

const std::vector<Molecule*>& Grid::getCell(int x, int y) const {
    return cells[x][y];
}