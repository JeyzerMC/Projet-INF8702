#ifndef SCENE_SOUS_MARINE_H
#define SCENE_SOUS_MARINE_H

#include <memory>
#include <vector>

#include "../utils/shader.h"
#include "../utils/model.h"
#include "../utils/camera.h"
#include "../utils/options.h"
#include "post_processing.h"
#include "shadowmap.h"
#include "transform.h"

struct SceneObject {
    Transform transform;
    std::shared_ptr<Model> model;
};

class Scene {
public:
    Scene(int scr_width, int scr_height);

    void processInputs(int key);

    void render(Camera* camera, double time);

    void reload_shaders();

private:
    void draw_models(Shader& shader);

    // Shaders
    Shader rend_shader;
    // Scene dimensions
    int scr_width, scr_height;

    // Models
    std::shared_ptr<Model> ground;
    std::shared_ptr<Model> pot;
    std::shared_ptr<Model> sand;
    // std::shared_ptr<Model> spiral_shell;
    // std::shared_ptr<Model> big_shell;
    // std::shared_ptr<Model> star_fish;
    std::shared_ptr<Model> fish;
    std::shared_ptr<Model> coral_red;
    std::shared_ptr<Model> coral_pink;
    std::shared_ptr<Model> coral_green;

    std::vector<SceneObject> objects;

    // Post Processing
    PostProcessing post_process;
    Shadowmap shadowmap;

    // Caustics
    // TODO: RE-ADD CAUSTICS
    // AnimatedTexture water_normals;

    // Lights
    glm::vec3 light_pos;

    Options options;
};

#endif //SCENE_SOUS_MARINE_H
