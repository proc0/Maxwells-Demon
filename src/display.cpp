#include "display.hpp"
#include "common.hpp"
#include "raylib.h"

void Display::Load() {
	
}

UIEvent Display::Update() {
	UIEvent event = {0};

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		const Vector2& mousePos = GetMousePosition();

		if ((INTERSECTS(mousePos, displayPanelTop)) || (INTERSECTS(mousePos, displayPanelBottom))) {
			if (INTERSECTS(mousePos, resetButton)) {
				event.reset = true;
			} else if (INTERSECTS(mousePos, pauseButton)) {
				event.pause = true;
			} else if (INTERSECTS(mousePos, addButton)) {
				event.add = true;
			} else if (INTERSECTS(mousePos, subButton)) {
				event.sub = true;
			}
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

	DrawRectangleRec(subButtonBorder, BLACK);
	DrawRectangleRec(subButton, RAYWHITE);
	DrawText(subText, SUB_BUTTON_PAD_X(SUB_BUTTON_X), SQUARE_PAD_Y(SUB_BUTTON_Y), FONT_SIZE, BLACK);

	DrawRectangleRec(addButtonBorder, BLACK);
	DrawRectangleRec(addButton, RAYWHITE);
	DrawText(addText, SQUARE_PAD_X(ADD_BUTTON_X), SQUARE_PAD_Y(ADD_BUTTON_Y), FONT_SIZE, BLACK);
}

void Display::Unload() {
	
}