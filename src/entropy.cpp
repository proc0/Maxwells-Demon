#include "entropy.hpp"
#include "common.hpp"


void Entropy::Load(ThermalCount thermal) {
    
    totalHotCount = thermal.leftHot + thermal.rightHot;
    totalCoolCount = thermal.leftCold + thermal.rightCold;
    float halfHotCount = totalHotCount/2.0f;
    float halfColdCount = totalCoolCount/2.0f;
    maxEntropy = calculateBoltzmannEntropy(halfHotCount, totalHotCount - halfHotCount, halfColdCount, totalCoolCount - halfColdCount);
    entropy = calculateBoltzmannEntropy(thermal.leftHot, thermal.rightHot, thermal.leftCold, thermal.rightCold);
    completion = entropy / maxEntropy;
}

void Entropy::Update(ThermalCount thermal) {
    entropy = calculateBoltzmannEntropy(thermal.leftHot, thermal.rightHot, thermal.leftCold, thermal.rightCold);
    completion = entropy / maxEntropy;
    entropyBar = entropyBarLength * completion;
    barColor = ColorLerp(GREEN, GRAY, completion);
}

void Entropy::Render(float leftChamberCoolCount, float leftChamberHotCount, float rightChamberCoolCount, float rightChamberHotCount) const {
    const char *leftHotCountText = TextFormat("%.f", leftChamberHotCount);
    const char *leftColdCountText = TextFormat("%.f", leftChamberCoolCount);
    const char *rightHotCountText = TextFormat("%.f", rightChamberHotCount);
    const char *rightColdCountText = TextFormat("%.f", rightChamberCoolCount);

    DrawText(leftHotCountText, 280, 15, 20, RED);
    DrawText(pipeText, 300, 15, 20, BLACK); 
    DrawText(leftColdCountText, 308, 15, 20, BLUE);

    DrawText(rightHotCountText, 948, 15, 20, RED);
    DrawText(pipeText, 970, 15, 20, BLACK);
    DrawText(rightColdCountText, 980, 15, 20, BLUE);

    const char *entText = TextFormat("Entropy %.2f", entropy);
    const char *maxEntText = TextFormat("Max %.2f", maxEntropy);
    DrawText(entText, 430, 15, 20, BLACK);
    DrawText(maxEntText, 730, 15, 20, BLACK);

    DrawRectangle(entropyBarX-1, entropyBarY-1, entropyBarLength+2, 7, BLACK);
    DrawRectangle(entropyBarX, entropyBarY, entropyBarLength, 5, RAYWHITE);
    DrawRectangle(entropyBarX, entropyBarY, entropyBar, 5, barColor);
}

void Entropy::Unload() {

}

float Entropy::calculateShannonEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const
{
    // Stirling Approx. : n*(N!) ≈ N*lnN − N
    // This has negative results (error approx.) for small numbers
    float totalCountHot = leftHotCount + rightHotCount;
    float totalCountCold = leftColdCount + rightColdCount;
    float p = leftHotCount / totalCountHot;
    float q = rightHotCount / totalCountHot;
    float r = leftColdCount / totalCountCold;
    float t = rightColdCount / totalCountCold;

    float leftHotMember = p != 1 && p != 0 ? totalCountHot * (p * log(p) + (1 - p) * log(1 - p)) : 0;
    float leftCoolMember = r != 1 && r != 0 ? totalCountCold * (r * log(r) + (1 - r) * log(1 - r)) : 0;
    float leftEntropy = leftHotMember == 0 ? leftCoolMember : -leftHotMember - leftCoolMember;

    float rightHotMember = q != 1 && q != 0 ? totalCountHot * (q * log(q) + (1 - q) * log(1 - q)) : 0;
    float rightCoolMember = t != 1 && t != 0 ? totalCountCold * (t * log(t) + (1 - t) * log(1 - t)) : 0;
    float rightEntropy = rightHotMember == 0 ? rightCoolMember : -rightHotMember - rightCoolMember;

    return leftEntropy + rightEntropy;
}

float factorial(const int n)
{
    float f = 1;
    for (int i = 1; i <= n; ++i)
        f *= i;
    return f;
}

float Entropy::calculateBoltzmannEntropy(float leftHotCount, float rightHotCount, float leftColdCount, float rightColdCount) const
{   // S = k*ln*W
    // W = N1! / N2!*N3!
    // where N1 is total count of molecules, N2, N3 are chamber counts
    float totalCountHot = leftHotCount + rightHotCount;
    float totalCountCold = leftColdCount + rightColdCount;
    float hotEntropy = factorial(totalCountHot) / (factorial(leftHotCount) * factorial(rightHotCount));
    float coldEntropy = factorial(totalCountCold) / (factorial(leftColdCount) * factorial(rightColdCount));

    return log(hotEntropy * coldEntropy);
}