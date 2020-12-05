#ifndef EROSION_H
#define EROSION_H

#include <glad/glad.h>
#include <vector>

#include "../utils/shader.h"
#include "../utils/options.h"

#include "screen_pass.h"

class Erosion: public ScreenPass {

public:
    Erosion();

    void render(const Options& options, const glm::vec3& camPos);
    void reload();

private:
    Shader erode_shader;
};
#endif //EROSION_H
