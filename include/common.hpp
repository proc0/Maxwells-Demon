#pragma once

#include <raylib.h>
#include <raymath.h>

#define CONTAINER_WIDTH 800
#define CONTAINER_HEIGHT 400
#define CONTAINER_X 240
#define CONTAINER_Y 200
#define CONTAINER_BORDER 15
#define CONTAINER_BORDER_WIDTH CONTAINER_WIDTH + CONTAINER_BORDER*2
#define CONTAINER_BORDER_HEIGHT CONTAINER_HEIGHT + CONTAINER_BORDER*2
#define CONTAINER_BORDER_X CONTAINER_X - CONTAINER_BORDER
#define CONTAINER_BORDER_Y CONTAINER_Y - CONTAINER_BORDER

#define EASE_OUT_EXPO(x) ((x) >= 1 ? 1 : 1 - pow(2, -10 * (x)))

typedef struct ThermalCount {
    float leftHot = 0;
    float leftCold = 0;
    
    float rightHot = 0;
    float rightCold = 0;
} ThermalCount;

namespace Locate {
    float Top(Vector2 position, float offset);
    float Left(Vector2 position, float offset);
    float Right(Vector2 position, float offset);
    float Bottom(Vector2 position, float offset);
};
