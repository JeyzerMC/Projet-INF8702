#ifndef UNDERWATER_H
#define UNDERWATER_H

#include <glad/glad.h>
#include "../utils/shader.h"
#include "../utils/options.h"

#include <vector>

class Underwater {

public:
    Underwater();
    ~Underwater();

    void render(const Options& options, const glm::vec3& camPos);
    void bind();
    void reload();

private:
    void initBuffers();
    void initShaders();
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_color;
    Shader uw_shader;
};
#endif //UNDERWATER_H
