#include "options.h"

void Options::inputReceived(int key) 
{
    // Toggle inputs - one time fire offs
    if (key == 82) // R
        reloadShadersNextFrame = true;
    if (key == 86) { // V
        showToonShading = !showToonShading;
        showEffects = 1;
    }
    if (key == 67) { // C
        showCaustics = !showCaustics;
        showEffects = 1;
    }
    if (key == 66) { // B
        showEdges = !showEdges;
        showEffects = 1;
    }
    if (key == 78) { // N
        normalSmoothing = !normalSmoothing;
        showEffects = 1;
    }
    if (key == 77) { // M
        showWobbling = !showWobbling;
        showEffects = 1;
    }
    if (key == 74) { // J
        showBlur = !showBlur;
        showEffects = 1;
    }
    if (key == 75) { // K
        showTint = !showTint;
        showEffects = 1;
    }
    if (key == 49) // 1
        showEffects = 1; // Selected Effects 
    if (key == 50) // 2
        showEffects = 2; // All Effects 
    if (key == 51) // 3
        showEffects = 3; // No Effects
}