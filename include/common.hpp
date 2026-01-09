#pragma once

#include <array>
#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define DENSITY_START 18

#define MOLECULE_RADIUS 12.0f

#define CONTAINER_WIDTH 820
#define CONTAINER_HEIGHT 420
#define CONTAINER_X 230
#define CONTAINER_Y 190
#define CONTAINER_BORDER 10
#define CONTAINER_BORDER_WIDTH CONTAINER_WIDTH + CONTAINER_BORDER*2
#define CONTAINER_BORDER_HEIGHT CONTAINER_HEIGHT + CONTAINER_BORDER*2
#define CONTAINER_BORDER_X CONTAINER_X - CONTAINER_BORDER
#define CONTAINER_BORDER_Y CONTAINER_Y - CONTAINER_BORDER
#define CONTAINER_RIGHT (CONTAINER_X + CONTAINER_BORDER_WIDTH)
#define CONTAINER_BOTTOM (CONTAINER_Y + CONTAINER_BORDER_HEIGHT)

#define INVERSE_EXPONENTIAL(x) ((x) >= 1 ? 1 : 1 - pow(2, -10 * (x)))

static inline float factorial(float n) {
    float f = 1;
    for (float i = 1; i <= n; ++i) { 
        f *= i;
    }
    return f;
}

typedef struct Thermal {
    float leftHot = 0;
    float leftCold = 0;
    float rightHot = 0;
    float rightCold = 0;
} Thermal;

typedef struct Memo {
    Thermal stats;
} Memo;

namespace Locate {
    float Top(Vector2 point, float offset);
    float Left(Vector2 point, float offset);
    float Right(Vector2 point, float offset);
    float Bottom(Vector2 point, float offset);
};

typedef struct UIEvent {
    bool reset;
    bool pause;
    bool add;
    bool sub;
} UIEvent;

