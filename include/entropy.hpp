#pragma once

#include "common.hpp"

#define BAR_WIDTH 548
#define BAR_HEIGHT 5
#define BAR_X CONTAINER_X
#define BAR_Y 40
#define BAR_BORDER_WIDTH BAR_WIDTH+2
#define BAR_BORDER_HEIGHT 7
#define BAR_BORDER_X BAR_X-1
#define BAR_BORDER_Y BAR_Y-1

class Entropy {
    Thermal cache;
    const char *separatorText = "-";
    const char *entropyText = "Entropy";
	Color barColor = GRAY;
    float totalHot = 0;
    float totalCold = 0;
    float total = 0;
    float totalHotFact = 0;
    float totalColdFact = 0;
    float percent = 0.0f;
    float current = 0.0f;
    float maximum = 0.0f;
    int totalX = 630;
    int barLength = 0;

public:
	Entropy(){};
	~Entropy() = default;

	void Load(const Memo&);
    void Init(const Memo& memo); 
	bool Update(const Memo&);
	void Render(const Memo&) const;
	void Unload();
    float calculateShannonEntropy(const Thermal) const;
    float calculateBoltzmannEntropy(const Thermal) const;
};
