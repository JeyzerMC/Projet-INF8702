#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include "../utils/shader.h"

class PostProcessing {

public:
    PostProcessing(int scr_width, int scr_height);
    ~PostProcessing();

    void InitFBO(glm::vec3 lightPos);
    void bindFBO();
    void renderFBO(bool toonShading, bool caustics, int showEdges);
private:
    void initBuffers(int scr_width, int scr_height);
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_normal, g_color;
    Shader pp_shader;
};
#endif //POST_PROCESSING_H
