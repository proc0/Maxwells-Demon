#include "display.hpp"
#include "raylib.h"

void Display::Load() {
	
}

UIEvent Display::Update() {
	UIEvent event = { .reset = false };

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		const Vector2& mousePos = GetMousePosition();

		if (mousePos.x > resetButton.x && mousePos.x < resetButton.x + resetButton.width && mousePos.y > resetButton.y && mousePos.y < resetButton.y + resetButton.height) {
			event.reset = true;
		} else if (mousePos.x > pauseButton.x && mousePos.x < pauseButton.x + pauseButton.width && mousePos.y > pauseButton.y && mousePos.y < pauseButton.y + pauseButton.height) {
			event.pause = true;
		} else if (mousePos.x > addButton.x && mousePos.x < addButton.x + addButton.width && mousePos.y > addButton.y && mousePos.y < addButton.y + addButton.height) {
			event.add = true;
		} else if (mousePos.x > subButton.x && mousePos.x < subButton.x + subButton.width && mousePos.y > subButton.y && mousePos.y < subButton.y + subButton.height) {
			event.sub = true;
		}
	}

	return event;
}

void Display::Render() const {
	DrawRectangleRec(resetButton, GREEN);
	DrawText(resetText, resetButton.x+7, resetButton.y+7, 15, RAYWHITE);

	DrawRectangleRec(addButton, BLUE);
	DrawText(addText, addButton.x+7, addButton.y+7, 15, RAYWHITE);

	DrawRectangleRec(pauseButton, BLUE);
	DrawText(pauseText, pauseButton.x+7, pauseButton.y+7, 15, RAYWHITE);

	DrawRectangleRec(subButton, BLUE);
	DrawText(subText, subButton.x+7, subButton.y+7, 15, RAYWHITE);
}

void Display::Unload() {
	
}