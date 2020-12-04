#ifndef CONSTANTS_H
#define CONSTANTS_H


class Constants {
public:
    constexpr static unsigned int SCR_WIDTH = 800;
    constexpr static unsigned int SCR_HEIGHT = 600;

    constexpr static unsigned int NORMAL_SMOOTHNESS_LEVEL = 7;

    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    constexpr static float QUAD_VERTICES[] = { 
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
};

#endif // CONSTANTS_H