
#include "grid.hpp"

void Grid::Load(short width, short height, float unit) {
    cellSize = unit;
    short columns = static_cast<short>(ceil(static_cast<float>(width) / cellSize));
    short rows = static_cast<short>(ceil(static_cast<float>(height) / cellSize));

    cellCount = Vector2(columns, rows);
    cells.resize(columns, std::vector<Cell>(rows));

    for (short x = 0; x < columns; ++x) {
        for (short y = 0; y < rows; ++y) {
            cells[x][y] = Cell();
        }
    }
}

Vector2 Grid::GridPoint(float _x, float _y) const {
    short x = floor(static_cast<short>((_x - CONTAINER_X) / cellSize));
    short y = floor(static_cast<short>((_y - CONTAINER_Y) / cellSize));
    // clamp grid bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= cellCount.x) x = cellCount.x - 1;
    if (y >= cellCount.y) y = cellCount.y - 1;

    return Vector2(x, y);
}

void Grid::AddPoint(Vector2 point, short id) {
    Vector2 cell = GridPoint(point.x, point.y);
    cells[cell.x][cell.y].push_back(id);
}

void Grid::RemovePoint(Vector2 cell, short id) {
    auto gridCell = cells[cell.x][cell.y];
    auto newEnd = std::remove(gridCell.begin(), gridCell.end(), id);
    gridCell.erase(newEnd, gridCell.end());
}

Vector2 Grid::UpdatePoint(Vector2 point, Vector2 cell, short id) {
    Vector2 newCell = GridPoint(point.x, point.y);
    if (newCell.x != cell.x || newCell.y != cell.y) {
        RemovePoint(cell, id);
        cells[newCell.x][newCell.y].push_back(id);
        return newCell;
    }

    return Vector2(0, 0);
}

Cell Grid::ZonePoint(Vector2 point, float radius, short id) {
    Vector2 topLeft = GridPoint(Locate::Left(point, radius), Locate::Top(point, radius));
    Vector2 bottomRight = GridPoint(Locate::Right(point, radius), Locate::Bottom(point, radius));

    Cell zone;
    for (short x = topLeft.x; x <= bottomRight.x; ++x) {
        for (short y = topLeft.y; y <= bottomRight.y; ++y) {
            const Cell& cell = cells[x][y];
            for (auto cid : cell) {
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
                if (cacheIndex > MAX_CACHE_SIZE) {
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

void Grid::AddArea(Rectangle area, short id) {
    Vector2 cell = GridPoint(area.x, area.y);

    int widthCells = ceil(area.width / cellSize);
    int heightCells = ceil(area.height / cellSize);

    for (int x = cell.x; x < cell.x + widthCells; x++) {
        for (int y = cell.y; y < cell.y + heightCells; y++) {
            cells[x][y].push_back(id);
        }
    }
}

void Grid::RemoveArea(Rectangle area, short id) {
    Vector2 position = GridPoint(area.x, area.y);

    short widthCells = ceil(area.width / cellSize);
    short heightCells = ceil(area.height / cellSize);

    for (short x = position.x; x < position.x + widthCells; x++) {
        for (short y = position.y; y < position.y + heightCells; y++) {
            Cell& cell = cells[x][y];

            auto newEnd = std::remove(cell.begin(), cell.end(), id);
            cell.erase(newEnd, cell.end());
        }
    }
}

Cell Grid::ZoneArea(Vector2 point, float radius) const {
    Vector2 topLeft = GridPoint(Locate::Left(point, radius), Locate::Top(point, radius));
    Vector2 bottomRight = GridPoint(Locate::Right(point, radius), Locate::Bottom(point, radius));

    Cell zone;
    std::vector<short> processed;
    for (short x = topLeft.x; x <= bottomRight.x; ++x) {
        for (short y = topLeft.y; y <= bottomRight.y; ++y) {
            const Cell& cell = cells[x][y];
            for (auto cid : cell) {
                bool isProcessed = false;
                for (short procId : processed) {
                    if (procId == cid) {
                        isProcessed = true;
                        break;
                    }
                }

                if (!isProcessed) {
                    zone.push_back(cid);
                    processed.push_back(cid);
                }
            }
        }
    }

    return zone;
}

float Locate::Top(Vector2 point, float offset) { return point.y - offset; }
float Locate::Left(Vector2 point, float offset) { return point.x - offset; }
float Locate::Right(Vector2 point, float offset) { return point.x + offset; }
float Locate::Bottom(Vector2 point, float offset) { return point.y + offset; }
