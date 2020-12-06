#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include <vector>

#include "../utils/shader.h"
#include "../utils/options.h"
#include "shadowmap.h"
#include "caustics.h"
#include "underwater.h"
#include "erosion.h"
#include "dilation.h"

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
    unsigned int g_buffer;
    unsigned int g_position, g_normal, g_color, g_smooth;
    Shader pp_shader;

    Caustics caustics;
    
    Erosion erosion_pass;
    Dilation opening_dilation_pass;
    Dilation closing_dilation_pass;
    Underwater uw_pass;

    arno::Texture t_turbulent_flow;
    arno::Texture t_pigment_dispersion;
    arno::Texture t_paper_layer;
    arno::Texture t_abstract_colors;
    glm::vec3 light_pos;
};
#endif //POST_PROCESSING_H
