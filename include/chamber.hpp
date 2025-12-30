#pragma once

#include <array>
// #include <vector>
// #include <raylib.h>
// #include <raymath.h>

#include "common.hpp" // IWYU pragma: keep
#include "grid.hpp"

#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60

typedef struct Wall2
{
    Rectangle rect;
    int id;
} Wall2;

class Chamber {
    Color colorChamberLeft;
    Color colorChamberRight;
	Grid grid;
    Rectangle containerLeft;
    Rectangle containerRight;

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
    Cell GetWalls(Vector2 point, float radius) const;

    void Render() const;
	void Update();
	void UpdateColors(float leftChamberCoolCount, float leftChamberHotCount, float rightChamberCoolCount, float rightChamberHotCount, float totalCoolCount, float totalHotCount);
	void Unload();
};
