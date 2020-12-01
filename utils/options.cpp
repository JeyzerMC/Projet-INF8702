#include "options.h"

void Options::inputReceived(int key) 
{
    // Toggle inputs - one time fire offs
    if (key == 82) // R
        reloadShadersNextFrame = true;
    if (key == 86) { // V
        showToonShading = !showToonShading;
        showEffects = 0;
    }
    if (key == 67) { // C
        showCaustics = !showCaustics;
        showEffects = 0;
    }
    if (key == 66) { // B
        showEdges = !showEdges;
        showEffects = 0;
    }
    if (key == 78) { // N
        normalSmoothing = !normalSmoothing;
        showEffects = 0;
    }
    if (key == 77) { // M
        showWobbling = !showWobbling;
        showEffects = 0;
    }
    if (key == 74) { // J
        showBlur = !showBlur;
        showEffects = 0;
    }
    if (key == 75) { // K
        showTint = !showTint;
        showEffects = 0;
    }
    if (key == 76) { // L
        showWatercolorTextures = !showWatercolorTextures;
        showEffects = 0;
    }
    if (key == 49) // 1
        showEffects = 0; // Selected Effects 
    if (key == 50) // 2
        showEffects = 1; // All Effects 
    if (key == 51) // 3
        showEffects = 2; // No Effects
}