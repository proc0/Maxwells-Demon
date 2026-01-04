#pragma once

#include "common.hpp" // IWYU pragma: keep

class Display {
	Rectangle resetButton = Rectangle({ 600, 600, 150, 50 }); 
	const char* resetText = "Reset";

public:
	Display(){};
	~Display() = default;

	void Load();
	UIEvent Update();
	void Render() const;
	void Unload();
};
