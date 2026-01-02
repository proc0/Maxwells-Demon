#pragma once

#include "common.hpp" 
#include "grid.hpp"

#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60

typedef struct Wall {
    Rectangle rect;
    int id;
} Wall;

class Chamber {
    std::array<Wall, 3> walls = {
    	Wall({ Rectangle({633, 200, 15, 150}), 0 }),
    	Wall({ Rectangle({633, 350, 15, 100}), 1 }),
    	Wall({ Rectangle({633, 450, 15, 150}), 2 })
    };
	Grid grid;
	Rectangle sensor = Rectangle({610, 300, 60, 200});
    Rectangle chamberLeft = Rectangle({ CONTAINER_X, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    Rectangle chamberRight = Rectangle({ CONTAINER_X + CONTAINER_WIDTH/2.0f, CONTAINER_Y, CONTAINER_WIDTH/2.0f, CONTAINER_HEIGHT });
    Color colorChamberLeft = BLUE;
    Color colorChamberRight = BLUE;
    float totalHotCount = 0;
    float totalCoolCount = 0;
    int doorFrame = 0;
    bool isDoorClosing = false;

public:
	Chamber(){};
	~Chamber() = default;

    void Load();
    void Init(State);
    bool checkTunneling(Vector2 position, float radius) const;
    bool IsDetected(Vector2 position, float radius) const;
    bool IsLeft(Vector2 position, float radius) const;
    Cell GetWalls(Vector2 point, float radius) const;
    const Wall& GetWall(short id) const;

    void Render() const;
	void Update(State);
	void UpdateColors(Thermal);
	void Unload();
};
