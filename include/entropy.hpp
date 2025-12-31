#pragma once

#include "common.hpp" // IWYU pragma: keep

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

	void Load(ThermalCount);
	void Update(ThermalCount);
	void Render(const ThermalCount) const;
	void Unload();
    float calculateShannonEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const;
    float calculateBoltzmannEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const;
};
