#pragma once

#include "common.hpp"

#define MAXWELL_URI "assets/maxwell.png"
#define TOASTY_URI "assets/toasty.mp3"

#define MAXWELL_MAX_FRAMES 60;
#define MAXWELL_DESTINATION_X 1030
#define MAXWELL_DESTINATION_Y 505

class Maxwell {
	Texture2D maxwell;
    Sound toasty;
    const char* helpText = "Left click or press [SPACEBAR] to open chamber door";
    Vector2 maxwellPosition = Vector2({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) });
    short maxwellFrameIndex = 0;
    bool isToasty = false;
    bool isToastyPlayed = false;

public:
	Maxwell() {};
	~Maxwell() = default;

	void Load();
	void Init();
	void Update();
	void Render() const;
	void Toasty();
	void Unload();
};
