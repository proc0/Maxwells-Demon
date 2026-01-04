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
		} 
	}

	return event;
}

void Display::Render() const {
	DrawRectangleRec(resetButton, GREEN);
	DrawText(resetText, resetButton.x+7, resetButton.y+7, 15, RAYWHITE);

	DrawRectangleRec(pauseButton, BLUE);
	DrawText(pauseText, pauseButton.x+7, pauseButton.y+7, 15, RAYWHITE);
}

void Display::Unload() {
	
}