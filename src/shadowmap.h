//
// Created by arnaud on 2020-11-29.
//

#ifndef SCENE_SOUS_MARINE_SHADOWMAP_H
#define SCENE_SOUS_MARINE_SHADOWMAP_H

#include <functional>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "../utils/shader.h"

class Shadowmap {
public:
    Shadowmap(unsigned int width, unsigned int height);
    ~Shadowmap();

    // TODO: This should take a light position
    void draw_in_map(const std::function<void(Shader&)>& draw_function);

    [[nodiscard]] glm::mat4 get_light_matrix() const;

    unsigned width, height;

    GLuint fbo;
    arno::Texture texture;

    Shader shadow_shader;

    void reload_shaders();
};



#endif //SCENE_SOUS_MARINE_SHADOWMAP_H
