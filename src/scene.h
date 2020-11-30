#ifndef SCENE_SOUS_MARINE_H
#define SCENE_SOUS_MARINE_H

#include "../utils/shader.h"
#include "../utils/model.h"
#include "../utils/camera.h"
#include "post_processing.h"
#include "shadowmap.h"

class Scene {
public:
    Scene(int scr_width, int scr_height);

    void render(Camera* camera, bool toonShading, bool caustics, bool showWobbling, int edges, int smoothLevel, double time);

    void reload_shaders();

private:
    void draw_models(Shader& shader);

    // Shaders
    Shader rend_shader;
    // Scene dimensions
    int scr_width, scr_height;

    // Models
    Model ground;
    Model pot;
    // Model spiral_shell;
    // Model big_shell;
    // Model star_fish;
    // Model fish;
    // Model coral;

    // Post Processing
    PostProcessing post_process;
    Shadowmap shadowmap;

    // Caustics
    // TODO: RE-ADD CAUSTICS
    // AnimatedTexture water_normals;

    // Lights
    glm::vec3 light_pos;
};

#endif //SCENE_SOUS_MARINE_H
