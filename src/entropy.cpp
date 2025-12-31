#include "entropy.hpp"
#include "common.hpp"


void Entropy::Load(Thermal stats) {
    totalHotCount = stats.leftHot + stats.rightHot;
    totalCoolCount = stats.leftCold + stats.rightCold;
    float halfHotCount = totalHotCount / 2.0f;
    float halfColdCount = totalCoolCount / 2.0f;
    const Thermal maxStats = {
        .leftHot = halfHotCount,
        .leftCold = halfColdCount,
        .rightHot = totalHotCount - halfHotCount,
        .rightCold = totalCoolCount - halfColdCount,
    };
    maxEntropy = calculateBoltzmannEntropy(maxStats);
    entropy = calculateBoltzmannEntropy(stats);
    completion = entropy / maxEntropy;
}

void Entropy::Update(Thermal stats) {
    entropy = calculateBoltzmannEntropy(stats);
    completion = entropy / maxEntropy;
    entropyBar = entropyBarLength * completion;
    barColor = ColorLerp(GREEN, GRAY, completion);
}

void Entropy::Render(const Thermal stats) const {
    const char *leftHotCountText = TextFormat("%.f", stats.leftHot);
    const char *leftColdCountText = TextFormat("%.f", stats.leftCold);
    const char *rightHotCountText = TextFormat("%.f", stats.rightHot);
    const char *rightColdCountText = TextFormat("%.f", stats.rightCold);

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

float Entropy::calculateShannonEntropy(const Thermal stats) const
{
    // Stirling Approx. : n*(N!) ≈ N*lnN − N
    // This has negative results (error approx.) for small numbers
    float totalCountHot = stats.leftHot + stats.rightHot;
    float totalCountCold = stats.leftCold + stats.rightCold;
    float p = stats.leftHot / totalCountHot;
    float q = stats.rightHot / totalCountHot;
    float r = stats.leftCold / totalCountCold;
    float t = stats.rightCold / totalCountCold;

    float leftHotMember = p != 1 && p != 0 ? totalCountHot * (p * log(p) + (1 - p) * log(1 - p)) : 0;
    float leftCoolMember = r != 1 && r != 0 ? totalCountCold * (r * log(r) + (1 - r) * log(1 - r)) : 0;
    float leftEntropy = leftHotMember == 0 ? leftCoolMember : -leftHotMember - leftCoolMember;

    float rightHotMember = q != 1 && q != 0 ? totalCountHot * (q * log(q) + (1 - q) * log(1 - q)) : 0;
    float rightCoolMember = t != 1 && t != 0 ? totalCountCold * (t * log(t) + (1 - t) * log(1 - t)) : 0;
    float rightEntropy = rightHotMember == 0 ? rightCoolMember : -rightHotMember - rightCoolMember;

    return leftEntropy + rightEntropy;
}

float factorial(const float n)
{
    float f = 1;
    for (float i = 1; i <= n; ++i)
        f *= i;
    return f;
}

float Entropy::calculateBoltzmannEntropy(const Thermal stats) const
{   // S = k*ln*W
    // W = N1! / N2!*N3!
    // where N1 is total count of molecules, N2, N3 are chamber counts
    float totalCountHot = stats.leftHot + stats.rightHot;
    float totalCountCold = stats.leftCold + stats.rightCold;
    float hotEntropy = factorial(totalCountHot) / (factorial(stats.leftHot) * factorial(stats.rightHot));
    float coldEntropy = factorial(totalCountCold) / (factorial(stats.leftCold) * factorial(stats.rightCold));

    return log(hotEntropy * coldEntropy);
}