//
// Created by arnaud on 2020-11-24.
//

#ifndef SCENE_SOUS_MARINE_ANIMATED_TEXTURE_H
#define SCENE_SOUS_MARINE_ANIMATED_TEXTURE_H


#include <vector>

#include "texture.h"

enum LoopMode {
    Repeat,
    PingPong,
};

class AnimatedTexture {
public:
    explicit AnimatedTexture(std::vector<arno::Texture> textures, float framerate, float phase = 0);
    arno::Texture& sampleTexture(double time);

    LoopMode loop_mode = LoopMode::Repeat;

    float period;
    float phase;
    std::vector<arno::Texture> textures;
};


#endif //SCENE_SOUS_MARINE_ANIMATED_TEXTURE_H
