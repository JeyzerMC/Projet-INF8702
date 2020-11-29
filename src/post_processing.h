#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include <vector>

#include "../utils/shader.h"
#include "animated_texture.h"
#include "shadowmap.h"

class PostProcessing {

public:
    PostProcessing(int scrWidth, int scrHeight);
    ~PostProcessing();

    void InitFBO(glm::vec3 lightPos);
    void bindFBO();
    void renderFBO(bool toonShading, bool caustics, int showEdges, int smoothLevel, double time, const Shadowmap& shadow_map);
private:
    void initBuffers();
    void smoothNormals();
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_normal, g_color, g_smooth;
    Shader pp_shader;
    int scr_width, scr_height;

    AnimatedTexture water_normal_map;

    arno::Texture t_turbulent_flow;
    arno::Texture t_pigment_dispersion;
    arno::Texture t_paper_layer;
    arno::Texture t_abstract_colors;

    std::vector<int> n_offsets;
};
#endif //POST_PROCESSING_H
