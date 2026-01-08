#include "entropy.hpp"
#include "raylib.h"

void Entropy::Load(const Memo& memo) {
    Init(memo);
}

void Entropy::Init(const Memo& memo) {
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
    total = totalHot + totalCold;
    // centering total depending on digits
    totalX = total < 10 ? 635 : 630;
    current = calculateBoltzmannEntropy(cache);
    percent = current / maximum;
    barLength = BAR_WIDTH * percent;
    barColor = ColorLerp(LIME, YELLOW, percent);
}

bool Entropy::Update(const Memo& memo) {
    if ( cache.leftHot == memo.stats.leftHot && 
        cache.leftCold == memo.stats.leftCold && 
        cache.rightHot == memo.stats.rightHot && 
        cache.rightCold == memo.stats.rightCold
    ) { return false; }

    cache = memo.stats;
    total = memo.stats.leftHot + memo.stats.rightHot + memo.stats.leftCold + memo.stats.rightCold;
    totalX = total < 10 ? 635 : 630;
    current = calculateBoltzmannEntropy(cache);
    percent = current / maximum;
    barLength = BAR_WIDTH * percent;
    barColor = ColorLerp(LIME, YELLOW, percent);

    return current <= 0.0f;
}

void Entropy::Render(const Memo& memo) const {
    const Thermal& stats = memo.stats;

    const char *leftHotText = TextFormat("%.f", stats.leftHot);
    DrawText(leftHotText, stats.leftHot < 10 ? 390 : 380, 635, 20, RED);
    DrawText(separatorText, 415, 635, 20, BLACK); 
    const char *leftColdText = TextFormat("%.f", stats.leftCold);
    DrawText(leftColdText, 440, 635, 20, BLUE);

    const char *totalText = TextFormat("%.f", total);
    DrawText(totalText, totalX, 635, 20, BLACK);

    const char *rightHotText = TextFormat("%.f", stats.rightHot);
    DrawText(rightHotText, stats.rightHot < 10 ? 830 : 820, 635, 20, RED);
    DrawText(separatorText, 855, 635, 20, BLACK);
    const char *rightColdText = TextFormat("%.f", stats.rightCold);
    DrawText(rightColdText, 880, 635, 20, BLUE);

    const char *entropy = TextFormat("%.2f", current);
    DrawText(entropy, CONTAINER_X, 15, 20, BLACK);
    DrawText(entropyText, 450, 15, 20, BLACK);
    const char *maxEntropy = TextFormat("%.2f", maximum);
    DrawText(maxEntropy, 735, 15, 20, BLACK);

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
