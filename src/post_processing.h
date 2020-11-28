#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include <vector>

#include "../utils/shader.h"

class PostProcessing {

public:
    PostProcessing(int scrWidth, int scrHeight);
    ~PostProcessing();

    void InitFBO(glm::vec3 lightPos);
    void bindFBO();
    void renderFBO(bool toonShading, bool caustics, int showEdges, int smoothLevel);
private:
    void initBuffers();
    void smoothNormals();
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_normal, g_color, g_smooth;
    int scr_width, scr_height;
    Shader pp_shader;

    std::vector<int> n_offsets;
};
#endif //POST_PROCESSING_H
