#ifndef UNDERWATER_H
#define UNDERWATER_H

#include <glad/glad.h>
#include "../utils/shader.h"
#include "../utils/options.h"

#include <vector>

class Underwater {

public:
    Underwater(int scr_width, int scr_height);
    ~Underwater();

    void render(const Options& options);
    void bind();
    void reload();

private:
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_color;
    Shader uw_shader;

    int scr_width, scr_height;
};
#endif //UNDERWATER_H
