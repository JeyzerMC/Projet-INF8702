#ifndef UNDERWATER_H
#define UNDERWATER_H

#include <glad/glad.h>
#include "../utils/shader.h"
#include "../utils/options.h"

#include "screen_pass.h"

#include <vector>

class Underwater: public ScreenPass {

public:
    Underwater();

    void render(const Options& options, const glm::vec3& camPos);
    void reload();

private:
    Shader uw_shader;
};
#endif //UNDERWATER_H
