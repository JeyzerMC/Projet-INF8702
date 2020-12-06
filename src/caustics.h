//
// Created by arnaud on 2020-11-29.
//

#ifndef SCENE_SOUS_MARINE_CAUSTICS_H
#define SCENE_SOUS_MARINE_CAUSTICS_H

#include <glad/glad.h>
#include "texture.h"
#include "../utils/shader.h"

class Caustics {
public:
    Caustics(unsigned width, unsigned height);
    ~Caustics();

    void render(double time);

    unsigned width;
    unsigned height;

    GLuint fbo;
    Texture caustics_texture;

    Shader shader;

    GLuint grid_vao;
    GLuint grid_vbo;
    GLuint grid_ibo;
    unsigned indices_number;

    void reload_shaders();
};


#endif //SCENE_SOUS_MARINE_CAUSTICS_H
