#include "chamber.hpp"

void Chamber::Render() const
{

    DrawRectangle(CONTAINER_BORDER_X, CONTAINER_BORDER_Y, CONTAINER_BORDER_WIDTH, CONTAINER_BORDER_HEIGHT, BLACK);
    DrawRectangle(CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH, CONTAINER_HEIGHT, RAYWHITE);

    for (const Wall2 &wall : walls)
    {
        DrawRectangleRec(wall.rect, BLACK);
    }
}

void Chamber::Update() {
    Wall2 *wall = &walls[1];

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
