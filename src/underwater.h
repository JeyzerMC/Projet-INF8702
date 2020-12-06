#ifndef UNDERWATER_H
#define UNDERWATER_H

#include <glad/glad.h>
#include "../utils/shader.h"
#include "../utils/options.h"

#include "screen_pass.h"
#include "texture.h"

#include <vector>

class Underwater: public ScreenPass {

public:
    Underwater();

    void render(const Options& options, const glm::vec3& camPos);
    void reload();

private:
    Shader uw_shader;

    Texture t_turbulent_flow;
    Texture t_pigment_dispersion;
    Texture t_paper_layer;
};
#endif //UNDERWATER_H
