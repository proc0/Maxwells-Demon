#pragma once

#include "common.hpp"

#define BAR_WIDTH (CONTAINER_BORDER_WIDTH - 100)
#define BAR_HEIGHT 5
#define BAR_X (CONTAINER_BORDER_X + 50)
#define BAR_Y 40
#define BAR_BORDER_WIDTH BAR_WIDTH+2
#define BAR_BORDER_HEIGHT 7
#define BAR_BORDER_X BAR_X-1
#define BAR_BORDER_Y BAR_Y-1

class Entropy {
    Thermal cache;
    const char *colon = ":";
	Color barColor = GRAY;
    float totalHot = 0;
    float totalCold = 0;
    float totalHotFact = 0;
    float totalColdFact = 0;
    float percent = 0.0f;
    float current = 0.0f;
    float maximum = 0.0f;
    int barLength = 0;

public:
	Entropy(){};
	~Entropy() = default;

	void Load(State);
	void Update(State);
	void Render(const State) const;
	void Unload();
    float calculateShannonEntropy(const Thermal) const;
    float calculateBoltzmannEntropy(const Thermal) const;
};
