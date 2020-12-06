//
// Created by arnaud on 2020-11-29.
//

#include <glm/gtc/matrix_transform.hpp>
#include "shadowmap.h"

Texture gen_fbo_texture(unsigned int width, unsigned int height) {
    auto texture_data = TextureData {
        nullptr,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
    };
    auto parameters = TextureParameters::get_default();
    parameters.texture_format = GL_DEPTH_COMPONENT;
    parameters.min_filter = GL_NEAREST;
    parameters.max_filter = GL_NEAREST;
    // TODO: We probably don't want repeat here :/
    parameters.wrap_mode_s = GL_REPEAT;
    parameters.wrap_mode_t = GL_REPEAT;
    return Texture(texture_data, width, height, parameters);
}

Shadowmap::Shadowmap(unsigned int width, unsigned int height)
    : width(width)
    , height(height)
    , fbo(0)
    , texture(gen_fbo_texture(width, height))
    ,shadow_shader(Shader("shaders/shadow.vs", "shaders/shadow.fs"))
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

Shadowmap::~Shadowmap() {
    glDeleteFramebuffers(1, &fbo);
}

void Shadowmap::draw_in_map(const std::function<void(Shader&)>& draw_function) {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    shadow_shader.use();
    shadow_shader.setMat4("lightMatrix", get_light_matrix());

    draw_function(shadow_shader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 Shadowmap::get_light_matrix() const {
    float near_plane = 1.0f, far_plane = 30.0f;
    glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(glm::vec3(0.0f, 20.0f, 0.0f),
                                       glm::vec3( 0.0f, 0.0f,  0.0f),
                                       glm::vec3( 0.0f, 0.0f,  1.0f));
    return light_projection * light_view;
}

void Shadowmap::reload_shaders() {
    shadow_shader.reload();
}
