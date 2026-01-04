#pragma once

#include "common.hpp" // IWYU pragma: keep

class Display {
	Rectangle resetButton = Rectangle({ 400, 650, 100, 30 }); 
	Rectangle pauseButton = Rectangle({ 700, 650, 100, 30 }); 
	Rectangle subButton = Rectangle({ 650, 650, 30, 30 }); 
	Rectangle addButton = Rectangle({ 850, 650, 30, 30 }); 
	const char* resetText = "Reset";
	const char* pauseText = "Pause";
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
