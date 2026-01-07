#pragma once

#include "common.hpp"

#define DEMON_ATLAS_URI "assets/demon2.png"

#define MAX_FRAMES 12
#define EXPONENTIAL(frame) INVERSE_EXPONENTIAL((1.0f / (frame)) * MAX_FRAMES)

constexpr std::array<short, MAX_FRAMES+1> generate_frames() {
    std::array<short, MAX_FRAMES+1> frames{};
    for (short i = MAX_FRAMES; i > 0; --i) {
        frames[i] = EXPONENTIAL(i);
    }
    return frames;
}

constexpr const auto FRAMES = generate_frames();

class Demon {
	Texture2D demonAtlas;
	Rectangle frame = Rectangle({ 0, 0, 150, 188 });
	Vector2 position = Vector2({ 500, 10 });
	int framePosition = 0;
	int frameCount = 0;
	int frameSpeed = 2;
	bool isLoweringArm = false;

public:
	Demon(){};
	~Demon() = default;

	void Load();
	void Update();
	void Render() const;
	void Unload();
};
