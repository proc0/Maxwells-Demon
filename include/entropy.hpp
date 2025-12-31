#pragma once

#include "common.hpp"

class Entropy {
	Color barColor = GRAY;
    const char *pipeText = ":";
    float totalHotCount = 0;
    float totalCoolCount = 0;
    float completion = 0.0f;
    float entropy = 0.0f;
    float maxEntropy = 0.0f;
    int entropyBarLength = CONTAINER_BORDER_WIDTH - 100;
    int entropyBar = 0;
    int entropyBarX = CONTAINER_BORDER_X + 50;
    int entropyBarY = 40;

public:
	Entropy(){};
	~Entropy() = default;

	void Load(Thermal);
	void Update(Thermal);
	void Render(const Thermal) const;
	void Unload();
    float calculateShannonEntropy(const Thermal) const;
    float calculateBoltzmannEntropy(const Thermal) const;
};
