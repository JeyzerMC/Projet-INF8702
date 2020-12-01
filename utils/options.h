#ifndef UNDERWATER_OPTIONS_H
#define UNDERWATER_OPTIONS_H

class Options {
public:
    void inputReceived(int key);
    int showEffects = 0;

    bool debug = true;
    bool showToonShading = true;
    bool showCaustics = true;
    bool showWobbling = true;
    bool reloadShadersNextFrame = false;
    bool showBlur = true;
    bool showTint = true;
    bool showEdges = true;
    bool normalSmoothing = true;
};

#endif //UNDERWATER_OPTIONS_H