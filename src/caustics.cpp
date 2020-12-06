//
// Created by arnaud on 2020-11-29.
//

#include <vector>
#include "caustics.h"

namespace {
    Texture gen_fbo_texture(unsigned int width, unsigned int height) {
        auto texture_data = TextureData{
                nullptr,
                GL_RGB,
                GL_FLOAT,
        };
        auto parameters = TextureParameters::get_default();
        parameters.texture_format = GL_RGB; // TODO: Memory optimisation - use only R component
        parameters.min_filter = GL_LINEAR;
        parameters.max_filter = GL_LINEAR;
        parameters.wrap_mode_s = GL_REPEAT;
        parameters.wrap_mode_t = GL_REPEAT;
        return Texture(texture_data, width, height, parameters);
    }
}

Caustics::Caustics(unsigned int width, unsigned int height)
    : width(width)
    , height(height)
    , fbo(0)
    , caustics_texture(::gen_fbo_texture(width, height))
    , shader("shaders/caustics.vs", "shaders/caustics.fs")
    , grid_vao(0)
    , grid_vbo(0)
    , grid_ibo(0)
    , indices_number(0)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, caustics_texture.texture, 0);

    glGenVertexArrays(1, &grid_vao);
    glBindVertexArray(grid_vao);

    // Generate a square grid from a to b with 10x10 verts
    float a = -1.2;
    float b = 1.2;
    unsigned resolution = 256;

    std::vector<float> vertex_data;
    vertex_data.resize(resolution * resolution * 3);
    for (unsigned i = 0; i < resolution; ++i) {
        for (unsigned j = 0; j < resolution; ++j) {
            unsigned index = (i + j * resolution) * 3;
            vertex_data[index + 0] = static_cast<float>(i) / static_cast<float>(resolution - 1) * (b - a) + a;
            vertex_data[index + 1] = static_cast<float>(j) / static_cast<float>(resolution - 1) * (b - a) + a;
            vertex_data[index + 2] = 0;
        }
    }
    glGenBuffers(1, &grid_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data(), GL_STATIC_DRAW);

    std::vector<unsigned> index_data;
    for (unsigned i = 0; i < resolution-1; ++i) {
        for (unsigned j = 0; j < resolution - 1; ++j) {
            index_data.push_back((i + 0) + (j + 0) * resolution);
            index_data.push_back((i + 1) + (j + 0) * resolution);
            index_data.push_back((i + 0) + (j + 1) * resolution);

            index_data.push_back((i + 1) + (j + 0) * resolution);
            index_data.push_back((i + 1) + (j + 1) * resolution);
            index_data.push_back((i + 0) + (j + 1) * resolution);
        }
    }
    indices_number = index_data.size();

    glGenBuffers(1, &grid_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * index_data.size(), index_data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
}

Caustics::~Caustics() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteBuffers(1, &grid_vbo);
    glDeleteBuffers(1, &grid_ibo);
    glDeleteVertexArrays(1, &grid_vao);
}

void Caustics::render(double time) {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    shader.setFloat("time", time);
    glBindVertexArray(grid_vao);
    glDrawElements(GL_TRIANGLES, indices_number, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Caustics::reload_shaders() {
    shader.reload();
}















