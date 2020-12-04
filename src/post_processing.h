#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include <vector>

#include "../utils/shader.h"
#include "../utils/options.h"
#include "shadowmap.h"
#include "caustics.h"

class PostProcessing {

public:
    PostProcessing();
    ~PostProcessing();

    void InitFBO(glm::vec3 lightPos);
    void bindFBO();
    void renderFBO(const Options& options, double time, const Shadowmap& shadow_map, const glm::vec3& camPos);

    void reload_shaders();

private:
    void initBuffers();
    void initP2Buffers();
    void init_shader();
    unsigned int VAO, VBO;
    unsigned int g_buffer, g_buffer2;
    unsigned int g_position, g_normal, g_color, g_smooth;
    unsigned int g_position2, g_color2;
    Shader pp_shader;
    Shader pp2_shader;

    Caustics caustics;

    arno::Texture t_turbulent_flow;
    arno::Texture t_pigment_dispersion;
    arno::Texture t_paper_layer;
    arno::Texture t_abstract_colors;
    glm::vec3 light_pos;
};
#endif //POST_PROCESSING_H
