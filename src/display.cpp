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
	DrawRectangleRec(pauseButtonBorder, BLACK);
	DrawRectangleRec(pauseButton, RAYWHITE);
	DrawText(pauseText, RECTANGLE_PAD_X(PAUSE_BUTTON_X), RECTANGLE_PAD_Y(PAUSE_BUTTON_Y), FONT_SIZE, BLACK);

	DrawRectangleRec(resetButtonBorder, BLACK);
	DrawRectangleRec(resetButton, RAYWHITE);
	DrawText(resetText, RECTANGLE_PAD_X(RESET_BUTTON_X), RECTANGLE_PAD_Y(RESET_BUTTON_Y), FONT_SIZE, BLACK);

	DrawRectangleRec(subButton, BLUE);
	DrawText(subText, SUB_BUTTON_PAD_X(SUB_BUTTON_X), SQUARE_PAD_Y(SUB_BUTTON_Y), FONT_SIZE, RAYWHITE);

	DrawRectangleRec(addButton, RED);
	DrawText(addText, SQUARE_PAD_X(ADD_BUTTON_X), SQUARE_PAD_Y(ADD_BUTTON_Y), FONT_SIZE, RAYWHITE);
}

void Display::Unload() {
	
}