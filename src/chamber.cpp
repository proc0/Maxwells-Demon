#include "chamber.hpp"

void Chamber::Load() {
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, MOLECULE_RADIUS * 4);
    for (short i = 0; auto& wall : walls) {
        grid.AddArea(wall.rect, i);
        i++;
    }

}

void Chamber::Init(const Memo& memo) {
    const Thermal& stats = memo.stats;
    totalHotCount = stats.leftHot + stats.rightHot;
    totalCoolCount = stats.leftCold + stats.rightCold;
}

void Chamber::Render() const {
    DrawRectangle(CONTAINER_BORDER_X, CONTAINER_BORDER_Y, CONTAINER_BORDER_WIDTH, CONTAINER_BORDER_HEIGHT, BLACK);
    DrawRectangle(CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH, CONTAINER_HEIGHT, RAYWHITE);

    DrawRectangleRec(chamberLeft, colorChamberLeft);
    DrawRectangleRec(chamberRight, colorChamberRight);

    for (const Wall& wall : walls) {
        DrawRectangleRec(wall.rect, BLACK);
    }
}

void Chamber::UpdateColors(Thermal stats) {
    if(stats.leftCold == stats.leftHot) {
        colorChamberLeft = RAYWHITE;
    } else {
        colorChamberLeft = Fade(stats.leftCold > stats.leftHot ? ColorLerp(RAYWHITE, BLUE, stats.leftCold/totalCoolCount) : ColorLerp(RAYWHITE, RED, stats.leftHot/totalHotCount), 0.2f);
    }

    if(stats.rightCold == stats.rightHot) {
        colorChamberRight = RAYWHITE;
    } else {
        colorChamberRight = Fade(stats.rightCold > stats.rightHot ? ColorLerp(RAYWHITE, BLUE, stats.rightCold/totalCoolCount) : ColorLerp(RAYWHITE, RED, stats.rightHot/totalHotCount), 0.2f);
    }
}

void Chamber::Update(const Memo& memo) {
    const Thermal& stats = memo.stats;
    Wall& door = walls[1];

    if (IsKeyReleased(KEY_SPACE)) {
        isDoorClosing = true;
    }

    if (isDoorClosing) {
        if (doorFrame == 0 && door.rect.y < DOOR_MAX_Y) {
            doorFrame = EASE_OUT_MAX_FRAMES;
        }

        if (doorFrame > 0) {
            door.rect.y += EASE_OUT_FRAMES[doorFrame] * 10;
            if (door.rect.y >= DOOR_MAX_Y) {
                door.rect.y = DOOR_MAX_Y;
                isDoorClosing = false;
            }
        }
    }

    if (IsKeyDown(KEY_SPACE)) {
        isDoorClosing = false;

        if (doorFrame == 0 && door.rect.y > DOOR_MIN_Y) {
            doorFrame = EASE_OUT_MAX_FRAMES;
        }

        if (doorFrame > 0) {
            door.rect.y -= EASE_OUT_FRAMES[doorFrame] * 10;
            if (door.rect.y <= DOOR_MIN_Y) {
                door.rect.y = DOOR_MIN_Y;
            }

            doorFrame--;
        }
    }

    UpdateColors(stats);
}

bool Chamber::checkTunneling(Vector2 position, float radius) const
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

bool Chamber::IsDetected(Vector2 position, float radius) const {
    int widthCells = ceil(sensor.width / 10);
    int heightCells = ceil(sensor.height / 10);

    bool isDetected = false;
    for (int x = sensor.x; x < sensor.x + widthCells; x++) {
        for (int y = sensor.y; y < sensor.y + heightCells; y++) {
            isDetected = CheckCollisionCircleRec(position, radius, sensor);
        }
    }

    return isDetected;
}

bool Chamber::IsLeft(Vector2 position, float radius) const {
    return CheckCollisionCircleRec(position, radius, chamberLeft);
}

Cell Chamber::GetWalls(Vector2 point, float radius) const {
    return grid.ZoneArea(point, radius);
}

const Wall& Chamber::GetWall(short id) const {
    return walls.at(id);
}

void Chamber::Unload() {

}
