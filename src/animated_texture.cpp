//
// Created by arnaud on 2020-11-24.
//

#include "animated_texture.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>

AnimatedTexture::AnimatedTexture(std::vector<arno::Texture> textures, float framerate, float phase)
    : period(static_cast<float>(textures.size()) * 1.0f / framerate)
    , phase(phase)
    , textures(std::move(textures))
{
    if (!std::isfinite(period) || std::abs(period) < 0.0001) {
        spdlog::error("Invalid period for animated texture. Period is {}", period);
    }
}

arno::Texture &AnimatedTexture::sampleTexture(double time) {
    double phasedTime = time - (double) phase;
    double timeInPeriod = fmod(phasedTime, period);
    float normalizedTime = static_cast<float>(timeInPeriod) / period;
    normalizedTime = std::clamp(normalizedTime, 0.0f, 1.0f);
    normalizedTime = std::clamp(normalizedTime, 0.0f, 1.0f);
    auto index = (size_t) ((float)(textures.size() - 1) * normalizedTime);
    if (index >= textures.size()) {
        spdlog::error("sampling texture from time failed! index was {}, but it needs to be smaller than {}", textures.size());
    }
    return textures[index];
}
