#include "demon.hpp"

void Demon::Load() {
	demonAtlas = LoadTexture(DEMON_ATLAS_URI);
}

void Demon::Update() {
    // frameCount++;

    // if (frameCount >= (60/frameSpeed)) {
    //     frameCount = 0;
    //     framePosition++;

    //     if (framePosition > 24) framePosition = 0;

    //     frame.x = static_cast<float>(framePosition*frame.width);
    // }

    if (IsKeyReleased(KEY_SPACE)) {
        isLoweringArm = true;
    }

    if (isLoweringArm) {
        if (frame.x <= 0 || frameCount <= 0) {
            isLoweringArm = false;
            return;
        }

        frame.x -= FRAMES[frameCount] * frame.width;

        frameCount--;
    }

    if (IsKeyDown(KEY_SPACE)) {

        if (frame.x >= demonAtlas.width-frame.width) {
            // frame.x = demonAtlas.width-frame.width;
            return;
        }

        isLoweringArm = false;

        frame.x += FRAMES[frameCount] * frame.width;

        frameCount++;
    }

}

void Demon::Render() const {
	DrawTextureRec(demonAtlas, frame, position, WHITE);
}

void Demon::Unload() {
	UnloadTexture(demonAtlas);
}
