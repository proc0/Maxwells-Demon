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
	DrawRectangleRec(pauseButton, WHITE);
	DrawText(pauseText, pauseButton.x+5, pauseButton.y+5, 20, BLACK);

	DrawRectangleRec(resetButton, WHITE);
	DrawText(resetText, resetButton.x+5, resetButton.y+5, 20, BLACK);

	DrawRectangleRec(subButton, BLUE);
	DrawText(subText, subButton.x+10, subButton.y+5, 20, RAYWHITE);

	DrawRectangleRec(addButton, RED);
	DrawText(addText, addButton.x+10, addButton.y+5, 20, RAYWHITE);
}

void Display::Unload() {
	
}