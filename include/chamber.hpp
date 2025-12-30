#pragma once

#include <array>
// #include <vector>
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

#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60
#define EASE_OUT_EXPO(x) ((x) >= 1 ? 1 : 1 - pow(2, -10 * (x)))

typedef struct Wall2
{
    Rectangle rect;
    int id;
} Wall2;

class Chamber {

public:
    std::array<Wall2, 3> walls = {
    	Wall2({ 
    		.rect = Rectangle({633, 200, 15, 150}),
    		.id = 0,
    	}),
    	Wall2({
    		.rect = Rectangle({633, 350, 15, 100}),
    		.id = 1, 
    	}),
    	Wall2({
    		.rect = Rectangle({633, 450, 15, 150}),
    		.id = 2,
    	})
    };
	Rectangle sensor = Rectangle({610, 300, 60, 200});
    int doorFrame = 0;
    bool isDoorClosing = false;

	Chamber(){};
	~Chamber() = default;

    void Load();
    bool checkTunneling(Vector2 position, float radius) const;
    bool checkSensor(Vector2 position, float radius) const;
    
    void Render() const;
	void Update();
	void Unload();
};
