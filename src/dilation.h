#ifndef DILATION_H
#define DILATION_H

#include <glad/glad.h>
#include <vector>

#include "../utils/shader.h"
#include "../utils/options.h"

#include "screen_pass.h"

class Dilation: public ScreenPass {

public:
    Dilation();

    void render(const Options& options, const glm::vec3& camPos);
    void reload();

private:
    Shader dilate_shader;
};
#endif //DILATION_H
