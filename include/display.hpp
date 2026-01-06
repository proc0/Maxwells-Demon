#pragma once

#include "common.hpp" // IWYU pragma: keep

class Display {
	Rectangle resetButton = Rectangle({ 930, 20, 80, 30 }); 
	Rectangle pauseButton = Rectangle({ 830, 20, 80, 30 }); 
	Rectangle subButton = Rectangle({ 580, 630, 30, 30 }); 
	Rectangle addButton = Rectangle({ 670, 630, 30, 30 }); 
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
