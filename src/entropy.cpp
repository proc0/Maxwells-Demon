#include "entropy.hpp"
#include "raylib.h"

void Entropy::Load(const Memo& memo) {
    const Thermal& stats = memo.stats;
    cache = stats;
    totalHot = stats.leftHot + stats.rightHot;
    totalCold = stats.leftCold + stats.rightCold;
    // caching for Boltzmann entropy
    totalHotFact = factorial(totalHot);
    totalColdFact = factorial(totalCold);

    float halfHot = totalHot / 2.0f;
    float halfCold = totalCold / 2.0f;
    const Thermal maxStats = {
        .leftHot = halfHot,
        .leftCold = halfCold,
        .rightHot = totalHot - halfHot,
        .rightCold = totalCold - halfCold,
    };
    maximum = calculateBoltzmannEntropy(maxStats);
    // the rest shares calculations with Update
    current = calculateBoltzmannEntropy(cache);
    percent = current / maximum;
    barLength = BAR_WIDTH * percent;
    barColor = ColorLerp(LIME, DARKGRAY, percent);
}

void Entropy::Update(const Memo& memo) {
    if ( cache.leftHot == memo.stats.leftHot && 
        cache.leftCold == memo.stats.leftCold && 
        cache.rightHot == memo.stats.rightHot && 
        cache.rightCold == memo.stats.rightCold
    ) { return; }

    cache = memo.stats;
    current = calculateBoltzmannEntropy(cache);
    percent = current / maximum;
    barLength = BAR_WIDTH * percent;
    barColor = ColorLerp(LIME, DARKGRAY, percent);
}

void Entropy::Render(const Memo& memo) const {
    const Thermal& stats = memo.stats;
    const char *leftHot = TextFormat("%.f", stats.leftHot);
    const char *leftCold = TextFormat("%.f", stats.leftCold);
    const char *rightHot = TextFormat("%.f", stats.rightHot);
    const char *rightCold = TextFormat("%.f", stats.rightCold);

    DrawText(leftHot, 280, 15, 20, RED);
    DrawText(colon, 300, 15, 20, BLACK); 
    DrawText(leftCold, 308, 15, 20, BLUE);

    DrawText(rightHot, 948, 15, 20, RED);
    DrawText(colon, 970, 15, 20, BLACK);
    DrawText(rightCold, 980, 15, 20, BLUE);

    const char *ent = TextFormat("Entropy %.2f", current);
    const char *max = TextFormat("Max %.2f", maximum);
    DrawText(ent, 430, 15, 20, BLACK);
    DrawText(max, 730, 15, 20, BLACK);

    DrawRectangle(BAR_BORDER_X, BAR_BORDER_Y, BAR_BORDER_WIDTH, BAR_BORDER_HEIGHT, BLACK);
    DrawRectangle(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, RAYWHITE);
    DrawRectangle(BAR_X, BAR_Y, barLength, BAR_HEIGHT, barColor);
}

float Entropy::calculateBoltzmannEntropy(const Thermal stats) const
{   // S = k*ln*W
    // W = N1! / N2!*N3!
    // where N1 is total count of molecules, N2, N3 are chamber counts
    float hotEntropy = totalHotFact / (factorial(stats.leftHot) * factorial(stats.rightHot));
    float coldEntropy = totalColdFact / (factorial(stats.leftCold) * factorial(stats.rightCold));

    return log(hotEntropy * coldEntropy);
}

void Entropy::Unload() {

}

// float Entropy::calculateShannonEntropy(const Thermal stats) const {
//     // Stirling Approx. : n*(N!) ≈ N*lnN − N
//     // This has negative results (error approx.) for small numbers
//     float p = stats.leftHot / totalHot;
//     float q = stats.rightHot / totalHot;
//     float r = stats.leftCold / totalCold;
//     float t = stats.rightCold / totalCold;

//     float leftHotStates = p != 1 && p != 0 ? totalHot * (p * log(p) + (1 - p) * log(1 - p)) : 0;
//     float leftColdStates = r != 1 && r != 0 ? totalCold * (r * log(r) + (1 - r) * log(1 - r)) : 0;
//     float leftEntropy = leftHotStates == 0 ? leftColdStates : -leftHotStates - leftColdStates;

//     float rightHotStates = q != 1 && q != 0 ? totalHot * (q * log(q) + (1 - q) * log(1 - q)) : 0;
//     float rightColdStates = t != 1 && t != 0 ? totalCold * (t * log(t) + (1 - t) * log(1 - t)) : 0;
//     float rightEntropy = rightHotStates == 0 ? rightColdStates : -rightHotStates - rightColdStates;

//     return leftEntropy + rightEntropy;
// }
