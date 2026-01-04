#include "display.hpp"
#include "raylib.h"

void Display::Load() {
	
}

UIEvent Display::Update() {
	UIEvent event = { .reset = false };

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		const Vector2& mousePos = GetMousePosition();
		if (mousePos.x > resetButton.x && mousePos.x < resetButton.x + resetButton.width && mousePos.y > resetButton.y && mousePos.y < resetButton.y + resetButton.height) {
			event.reset = true;
		}
	}

	return event;
}

void Display::Render() const {
	DrawRectangleRec(resetButton, GREEN);
	DrawText(resetText, 620, 620, 15, BLACK);
}

void Display::Unload() {
	
}