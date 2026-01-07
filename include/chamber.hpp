#pragma once

#include "common.hpp" 
#include "grid.hpp"

#define DOOR_MIN_Y 250.0f
#define DOOR_MAX_Y 350.0f
#define DOOR_OPEN_FRAMES 60

#define DOOR_MAX_FRAMES 59
#define DOOR_EASE_OUT(frame) INVERSE_EXPONENTIAL((1.0f / (frame)) * DOOR_MAX_FRAMES)

constexpr std::array<short, DOOR_MAX_FRAMES+1> generate_ease_out_frames() {
    std::array<short, DOOR_MAX_FRAMES+1> frames{};
    for (short i = DOOR_MAX_FRAMES; i > 0; --i) {
        frames[i] = DOOR_EASE_OUT(i);
    }
    return frames;
}

constexpr const auto DOOR_FRAMES = generate_ease_out_frames();

typedef struct Wall {
    Rectangle rect;
    int id;
} Wall;

class Chamber {
    std::array<Wall, 3> walls = {
    	Wall({ Rectangle({633, 190, 14, 160}), 0 }),
    	Wall({ Rectangle({633, 350, 14, 110}), 1 }),
    	Wall({ Rectangle({633, 460, 14, 160}), 2 })
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
    void Init(const Memo&);
    bool checkTunneling(Vector2 position, float radius) const;
    bool IsDetected(Vector2 position, float radius) const;
    bool IsLeft(Vector2 position, float radius) const;
    Cell GetWalls(Vector2 point, float radius) const;
    const Wall& GetWall(short id) const;

    void Render() const;
	void Update(const Memo&);
	void UpdateColors(Thermal);
	void Unload();
};
