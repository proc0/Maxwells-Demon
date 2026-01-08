#include "maxwell.hpp"

void Maxwell::Load() {
    maxwell = LoadTexture(MAXWELL_URI);
    toasty = LoadSound(TOASTY_URI);
}

void Maxwell::Init() {
	isToasty = false;
}

void Maxwell::Update() {

    if (isToasty) {
        maxwellFrameIndex++;

        if (!isToastyPlayed) {
            PlaySound(toasty);
            isToastyPlayed = true;
        }

        if (maxwellFrameIndex >= 120) {
            maxwellPosition.x += INVERSE_EXPONENTIAL(maxwellFrameIndex)*60;
            maxwellPosition.y += INVERSE_EXPONENTIAL(maxwellFrameIndex)*60;
            if (maxwellFrameIndex >= 240) {
                isToasty = false;
                isToastyPlayed = false;
                maxwellFrameIndex = 0;
                maxwellPosition = Vector2({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) });
            }
        } else if (maxwellPosition.x > MAXWELL_DESTINATION_X || maxwellPosition.y > MAXWELL_DESTINATION_Y) {
            maxwellPosition.x -= INVERSE_EXPONENTIAL(maxwellFrameIndex)*60;
            maxwellPosition.y -= INVERSE_EXPONENTIAL(maxwellFrameIndex)*60;
            if (maxwellPosition.x < MAXWELL_DESTINATION_X || maxwellPosition.y < MAXWELL_DESTINATION_Y) {
                maxwellPosition.x = MAXWELL_DESTINATION_X;
                maxwellPosition.y = MAXWELL_DESTINATION_Y;
            }
        }
    }

    // if (IsKeyReleased(KEY_SPACE)) {
    //     isToasty = true;
    // }
}

void Maxwell::Render() const {
    if (isToasty) {
        DrawTexture(maxwell, maxwellPosition.x, maxwellPosition.y, WHITE);
    }
}

void Maxwell::Toasty() {
	isToasty = true;
}

void Maxwell::Unload() {
    UnloadTexture(maxwell);
    UnloadSound(toasty);
}
