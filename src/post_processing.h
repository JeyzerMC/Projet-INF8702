#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H



#include <glad/glad.h>

#include "../utils/shader.h"

class PostProcessing {

public:
    PostProcessing(int scr_width, int scr_height);
    ~PostProcessing();

    void bindFBO();
    void renderFBO(int showEdges);
private:
    unsigned int VAO, VBO;
    unsigned int FBO, textureColorbuffer; // TODO: Remove those
    unsigned int g_buffer;
    unsigned int g_position, g_normal, g_color;
    Shader pp_shader;
};
#endif //POST_PROCESSING_H
