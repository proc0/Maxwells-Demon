#include "chamber.hpp"
#include "common.hpp"

void Chamber::Load() {

    containerLeft = Rectangle({ CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    containerRight = Rectangle({ CONTAINER_X + CONTAINER_WIDTH/2.0f, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    colorChamberLeft = BLUE;
    colorChamberRight = RED;
    // TODO: needs MOLECULE_RADIUS to be in global header of some kind
    grid.Load(CONTAINER_WIDTH, CONTAINER_HEIGHT, 12 * 4);
    grid.addArea(walls[0].rect, 0);
    grid.addArea(walls[1].rect, 1);
    grid.addArea(walls[2].rect, 2);
}

void Chamber::Init(ThermalCount thermal) {
    totalHotCount = thermal.leftHot + thermal.rightHot;
    totalCoolCount = thermal.leftCold + thermal.rightCold;
}

void Chamber::Render() const
{

    DrawRectangle(CONTAINER_BORDER_X, CONTAINER_BORDER_Y, CONTAINER_BORDER_WIDTH, CONTAINER_BORDER_HEIGHT, BLACK);
    DrawRectangle(CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH, CONTAINER_HEIGHT, RAYWHITE);

    DrawRectangleRec(containerLeft, colorChamberLeft);
    DrawRectangleRec(containerRight, colorChamberRight);

    for (const Wall &wall : walls)
    {
        DrawRectangleRec(wall.rect, BLACK);
    }
}

void Chamber::UpdateColors(ThermalCount thermal) {
    if(thermal.leftCold == thermal.leftHot) {
        colorChamberLeft = RAYWHITE;
    } else {
        colorChamberLeft = Fade(thermal.leftCold > thermal.leftHot ? ColorLerp(RAYWHITE, BLUE, thermal.leftCold/totalCoolCount) : ColorLerp(RAYWHITE, RED, thermal.leftHot/totalHotCount), 0.2f);
    }

    if(thermal.rightCold == thermal.rightHot) {
        colorChamberRight = RAYWHITE;
    } else {
        colorChamberRight = Fade(thermal.rightCold > thermal.rightHot ? ColorLerp(RAYWHITE, BLUE, thermal.rightCold/totalCoolCount) : ColorLerp(RAYWHITE, RED, thermal.rightHot/totalHotCount), 0.2f);
    }
}

void Chamber::Update(ThermalCount thermal) {
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

    UpdateColors(thermal);
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

bool Chamber::checkSensor(Vector2 position, float radius) const
{

    int widthCells = ceil(sensor.width / 10);
    int heightCells = ceil(sensor.height / 10);

    bool isDetected = false;
    for (int x = sensor.x; x < sensor.x + widthCells; x++)
    {
        for (int y = sensor.y; y < sensor.y + heightCells; y++)
        {
            isDetected = CheckCollisionCircleRec(position, radius, sensor);
        }
    }

    return isDetected;
}

Cell Chamber::GetWalls(Vector2 point, float radius) const {
    return grid.getArea(point, radius);
}

const Wall& Chamber::GetWall(short id) const {
    return walls.at(id);
}

void Chamber::Unload() {

}
