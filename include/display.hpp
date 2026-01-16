#pragma once

#include "common.hpp"

#define FONT_SIZE 20
#define BORDER_WIDTH 6
#define PAUSE_BUTTON_X 830
#define PAUSE_BUTTON_Y 23
#define RESET_BUTTON_X 945
#define RESET_BUTTON_Y 23
#define ADD_BUTTON_X 670
#define ADD_BUTTON_Y 630
#define SUB_BUTTON_X 580
#define SUB_BUTTON_Y 630
#define RECTANGLE_BUTTON_WIDTH 80
#define RECTANGLE_BUTTON_HEIGHT 30
#define SQUARE_BUTTON_WIDTH 30
#define SQUARE_BUTTON_HEIGHT 30
#define RECTANGLE_PAD_X(x) (x)+12
#define RECTANGLE_PAD_Y(y) (y)+5
#define SQUARE_PAD_X(x) (x)+10
#define SUB_BUTTON_PAD_X(x) (x)+11
#define SQUARE_PAD_Y(y) (y)+6

class Display {
	Rectangle resetButton = Rectangle({ RESET_BUTTON_X, RESET_BUTTON_Y, RECTANGLE_BUTTON_WIDTH, RECTANGLE_BUTTON_HEIGHT }); 
	Rectangle resetButtonBorder = Rectangle({ RESET_BUTTON_X - BORDER_WIDTH/2.0f, RESET_BUTTON_Y - BORDER_WIDTH/2.0f, RECTANGLE_BUTTON_WIDTH + BORDER_WIDTH, RECTANGLE_BUTTON_HEIGHT + BORDER_WIDTH }); 
	Rectangle pauseButton = Rectangle({ PAUSE_BUTTON_X, PAUSE_BUTTON_Y, RECTANGLE_BUTTON_WIDTH, RECTANGLE_BUTTON_HEIGHT }); 
	Rectangle pauseButtonBorder = Rectangle({ PAUSE_BUTTON_X - BORDER_WIDTH/2.0f, PAUSE_BUTTON_Y - BORDER_WIDTH/2.0f, RECTANGLE_BUTTON_WIDTH + BORDER_WIDTH, RECTANGLE_BUTTON_HEIGHT + BORDER_WIDTH }); 
	Rectangle subButton = Rectangle({ SUB_BUTTON_X, SUB_BUTTON_Y, SQUARE_BUTTON_WIDTH, SQUARE_BUTTON_HEIGHT }); 
	Rectangle subButtonBorder = Rectangle({ SUB_BUTTON_X - BORDER_WIDTH/2.0f, SUB_BUTTON_Y - BORDER_WIDTH/2.0f, SQUARE_BUTTON_WIDTH + BORDER_WIDTH, SQUARE_BUTTON_HEIGHT + BORDER_WIDTH });
	Rectangle addButton = Rectangle({ ADD_BUTTON_X, ADD_BUTTON_Y, SQUARE_BUTTON_WIDTH, SQUARE_BUTTON_HEIGHT });
	Rectangle addButtonBorder = Rectangle({ ADD_BUTTON_X - BORDER_WIDTH/2.0f, ADD_BUTTON_Y - BORDER_WIDTH/2.0f, SQUARE_BUTTON_WIDTH + BORDER_WIDTH, SQUARE_BUTTON_HEIGHT + BORDER_WIDTH });
	const char* resetText = "reset";
	const char* pauseText = "pause";
	const char* subText = "-";
	const char* addText = "+";

public:
	Display(){};
	~Display() = default;

	void Load();
	UIEvent Update();
	void Render() const;
	void Unload();
};
