#ifndef UNDERWATER_OPTIONS_H
#define UNDERWATER_OPTIONS_H

class Options {
public:
    void inputReceived(int key);
    int showEffects = 1; // Show all effects by default

    bool reloadShadersNextFrame = false;
    bool showToonShading = false;
    bool showCaustics = false;
    bool showWobbling = false;
    bool showWatercolorTextures = false;
    bool showBlur = false;
    bool showTint = false;
    bool showEdges = false;
    bool normalSmoothing = false;
};

#endif //UNDERWATER_OPTIONS_H