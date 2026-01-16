#include "demon.hpp"

void Demon::Load() {
	demonAtlas = LoadTexture(DEMON_ATLAS_URI);
}

void Demon::Update() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        const Vector2& mousePos = GetMousePosition();

        if ((INTERSECTS(mousePos, displayPanelTop)) || (INTERSECTS(mousePos, displayPanelBottom))) {
            return;
        }
    }

    if (IsKeyReleased(KEY_SPACE) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isLoweringArm = true;
    }

    if (isLoweringArm) {
        if (frame.x <= 0 || frameCount <= 0) {
            isLoweringArm = false;
            return;
        }

        frame.x -= DEMON_FRAMES[frameCount] * frame.width;

        frameCount--;
    }

    if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {

        if (frame.x >= demonAtlas.width-frame.width) {
            // frame.x = demonAtlas.width-frame.width;
            return;
        }

        isLoweringArm = false;

        frame.x += DEMON_FRAMES[frameCount] * frame.width;

        frameCount++;
    }

}

void Demon::Render() const {
	DrawTextureRec(demonAtlas, frame, position, WHITE);
}

void Demon::Unload() {
	UnloadTexture(demonAtlas);
}
