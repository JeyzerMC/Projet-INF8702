#ifndef CUBE__H
#define CUBE__H

#include <string>
#include <vector>

#include "../utils/shader.h"

#include <glm/glm.hpp>
#include <glad.h>
#include <fmt/format.h>

class Cube {
public:
    Cube();
    ~Cube();

    void render(Shader shader);

    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    unsigned int element_count;
};

#endif //CUBE_H
