#pragma once

#include "common.hpp"

#define DEMON_ATLAS_URI "assets/demon.png"

#define DEMON_MAX_FRAMES 12
#define DEMON_EASE_OUT(frame) INVERSE_EXPONENTIAL((1.0f / (frame)) * DEMON_MAX_FRAMES)

constexpr std::array<short, DEMON_MAX_FRAMES+1> generate_frames() {
    std::array<short, DEMON_MAX_FRAMES+1> frames{};
    for (short i = DEMON_MAX_FRAMES; i > 0; --i) {
        frames[i] = DEMON_EASE_OUT(i);
    }
    return frames;
}

constexpr const auto DEMON_FRAMES = generate_frames();

class Demon {
	Texture2D demonAtlas;
	Rectangle frame = Rectangle({ 0, 0, 123, 172 });
	Vector2 position = Vector2({ 530, 20 });
	int frameCount = 0;
	bool isLoweringArm = false;

public:
	Demon(){};
	~Demon() = default;

	void Load();
	void Update();
	void Render() const;
	void Unload();
};
