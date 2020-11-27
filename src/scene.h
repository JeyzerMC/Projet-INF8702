#ifndef SCENE_SOUS_MARINE_H
#define SCENE_SOUS_MARINE_H

#include "../utils/shader.h"
#include "../utils/model.h"
#include "../utils/camera.h"
#include "animated_texture.h"
#include "post_processing.h"

class Scene {
public:
    Scene(int scr_width, int scr_height);

    void render(Camera* camera, bool toonShading, bool caustics, bool edges);

private:
    // Scene dimensions
    int scr_width, scr_height;
    // Shaders
    Shader rend_shader;

    // Models
    Model ground;
    // Model spiral_shell;
    // Model big_shell;
    // Model star_fish;
    // Model fish;
    // Model pot;
    // Model coral;

    // Post Processing
    PostProcessing post_process;

    // Caustics
    AnimatedTexture water_normals;

    // Lights
    glm::vec3 light_pos;
};

#endif //SCENE_SOUS_MARINE_H