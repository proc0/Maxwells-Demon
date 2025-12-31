
#include "grid.hpp"
#include "raylib.h"

// GRID

void Grid::Load(short width, short height, float unit)
{
    cellSize = unit;
    short columns = static_cast<short>(ceil(static_cast<float>(width) / cellSize));
    short rows = static_cast<short>(ceil(static_cast<float>(height) / cellSize));

    cellCount = Vector2(columns, rows);
    cells.resize(columns, std::vector<Cell>(rows));
    // wallCells.resize(columns, std::vector<Cell>(rows));

    for (short x = 0; x < columns; ++x)
    {
        for (short y = 0; y < rows; ++y)
        {
            cells[x][y] = Cell();
            // wallCells[x][y] = Cell();
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
            cells[x][y].push_back(id);
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
            auto &cell = cells[x][y];

            auto newEnd = std::remove(cell.begin(), cell.end(), id);
            cell.erase(newEnd, cell.end());
        }
    }
}

Cell Grid::getArea(Vector2 point, float radius) const
{
    Vector2 topLeft = place(Locate::Left(point, radius), Locate::Top(point, radius));
    Vector2 bottomRight = place(Locate::Right(point, radius), Locate::Bottom(point, radius));

    Cell wallZone;
    std::vector<short> processed;
    for (short x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (short y = topLeft.y; y <= bottomRight.y; ++y)
        {
            auto cell = cells[x][y];
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

// void Grid::addWall(const Wall2& wall)
// {
//     Vector2 cell = place(wall.rect.x, wall.rect.y);

//     int widthCells = ceil(wall.rect.width / cellSize);
//     int heightCells = ceil(wall.rect.height / cellSize);

//     for (int x = cell.x; x < cell.x + widthCells; x++)
//     {
//         for (int y = cell.y; y < cell.y + heightCells; y++)
//         {
//             wallCells[x][y].push_back(wall.id);
//             // wall->cells.push_back(Vector2(x, y));
//         }
//     }
// }

// void Grid::removeWall(const Wall2& wall)
// {
//     Vector2 cell = place(wall.rect.x, wall.rect.y);

//     int widthCells = ceil(wall.rect.width / cellSize);
//     int heightCells = ceil(wall.rect.height / cellSize);

//     for (int x = cell.x; x < cell.x + widthCells; x++)
//     {
//         for (int y = cell.y; y < cell.y + heightCells; y++)
//         {
//             auto& wallCell = wallCells[x][y];

//             auto newEnd = std::remove(wallCell.begin(), wallCell.end(), wall.id);
//             wallCell.erase(newEnd, wallCell.end());
//         }
//     }
// }

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

    for (short x = topLeft.x; x <= bottomRight.x; ++x)
    {
        for (short y = topLeft.y; y <= bottomRight.y; ++y)
        {
            Cell cell = cells[x][y];
            for (auto cid : cell)
            {
                if (id == cid) continue;

                bool cacheHit = false;
                for (auto ccid : cache) {
                    if (ccid == cid) {
                        cacheHit = true;
                        break;
                    }
                }

                if (cacheHit) continue;

                cache[cacheIndex] = cid;
                if (cacheIndex > 4) {
                    cacheIndex = 0;
                } else {
                    cacheIndex++;
                }

                zone.push_back(cid);
            }
        }
    }

    return zone;
}

float Locate::Top(Vector2 point, float offset) { return point.y - offset; }
float Locate::Left(Vector2 point, float offset) { return point.x - offset; }
float Locate::Right(Vector2 point, float offset) { return point.x + offset; }
float Locate::Bottom(Vector2 point, float offset) { return point.y + offset; }

// Cell Grid::getWalls(Molecule *mol)
// {
//     Vector2 topLeft = place(Locate::Left(*mol), Locate::Top(*mol));
//     Vector2 bottomRight = place(Locate::Right(*mol), Locate::Bottom(*mol));

//     Cell wallZone;
//     std::vector<short> processed;
//     for (short x = topLeft.x; x <= bottomRight.x; ++x)
//     {
//         for (short y = topLeft.y; y <= bottomRight.y; ++y)
//         {
//             const auto &wallCell = wallCells[x][y];
//             for (auto cid : wallCell)
//             {
//                 bool isProcessed = false;
//                 for (short procId : processed)
//                 {
//                     if (procId == cid)
//                     {
//                         isProcessed = true;
//                         break;
//                     }
//                 }
//                 if (!isProcessed)
//                 {
//                     wallZone.push_back(cid);
//                     processed.push_back(cid);
//                 }
//             }
//         }
//     }

//     return wallZone;
// }







// Chamber stuff








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