#include "scene.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Scene::Scene(int w, int h)
  : rend_shader("shaders/camera.vs", "shaders/camera.fs"),
    scr_width(w), scr_height(h),
    ground(make_shared<Model>("models/Ground/Ground.obj", false)),
    pot(make_shared<Model>("models/Pot/Pot.obj")),
    sand(make_shared<Model>("models/Sand/Sand.obj")),
    spiral_shell(make_shared<Model>("models/Shells/SpiralShell.obj", false)),
    big_shell(make_shared<Model>("models/Shells/BigFanShell.obj")),
    star_fish(make_shared<Model>("models/StarFish/StarFish.obj")),
    fish(make_shared<Model>("models/Fishes/ClownFish.obj")),
    coral_red(make_shared<Model>("models/Coral/CoralRed.obj")),
    coral_pink(make_shared<Model>("models/Coral/CoralPink.obj")),
    coral_green(make_shared<Model>("models/Coral/CoralGreen.obj")),
    objects(),
    post_process(w, h),
    shadowmap(1024, 1024),
    light_pos(10.0, 50.0, 10.0)
{
    objects.push_back(SceneObject{
        Transform(glm::vec3(0, -1, 0), glm::quat(glm::vec3(0, 0, 0)), 0.5),
        ground,
    });

    objects.push_back(SceneObject{
        Transform(glm::vec3(-2, 0, -2), glm::quat(glm::vec3(0, -45, 0)), 0.2),
        pot,
    });

    objects.push_back(SceneObject{
        Transform(glm::vec3(4, 0, 3), glm::quat(glm::vec3(0, 0, 0)), 0.15),
        pot,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0, 3, 0), glm::quat(glm::vec3(0, 0, 0)), 4),
            fish,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0, 0.01, 0), glm::quat(glm::vec3(0, 0, 0)), 1),
            sand,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-3.5, 0, 3), glm::quat(glm::vec3(0, 45, 0)), 3),
            coral_red,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(3, 0, -1), glm::quat(glm::vec3(0, 0, 0)), 3),
            coral_red,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(2, 0, -3), glm::quat(glm::vec3(0, -55, 0)), 3),
            coral_pink,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-3, 0, -0.5), glm::quat(glm::vec3(0, 0, 0)), 5),
            coral_pink,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0.5, 0, 1.5), glm::quat(glm::vec3(0, 39, 0)), 4),
            coral_pink,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(4.2, 0, -3.7), glm::quat(glm::vec3(0, 70, 0)), 3),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-2, 0, 4.5), glm::quat(glm::vec3(0, -37, 0)), 2),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-3, 0, 1), glm::quat(glm::vec3(0, 14, 0)), 5),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0, 0, -3), glm::quat(glm::vec3(0, -25, 0)), 5),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-1, 0, -1), glm::quat(glm::vec3(0, 69, 0)), 3),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(3, 0, 1.5), glm::quat(glm::vec3(0, 35, 0)), 4),
            coral_green,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0.5, 0, 4.5), glm::quat(glm::vec3(0, 84, 0)), 3),
            coral_green,
    });

    // Shells
    objects.push_back(SceneObject{
            Transform(glm::vec3(-0.4, 0.3, 3.5), glm::quat(glm::vec3(0, 7, 0)), 5),
            spiral_shell,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(-1.3, 0.15, 2.3), glm::quat(glm::vec3(-0.03, 83, 0.02)), 4),
            big_shell,
    });

    // Star Fishes
    objects.push_back(SceneObject{
            Transform(glm::vec3(0.7, 0.1, -1.3), glm::quat(glm::vec3(-25, 24, 0)), 2),
            star_fish,
    });

    objects.push_back(SceneObject{
            // Transform(glm::vec3(4, 0.2, 4), glm::quat(glm::vec3(0, 3, 76)), 4),
            Transform(glm::vec3(4, 0.1, 4), glm::quat(glm::vec3(0, 3, 0.05)), 4),
            star_fish,
    });

//    water_normals.loop_mode = LoopMode::PingPong;
    glEnable(GL_DEPTH_TEST); // TODO: CHECK IF STAYS HERE
    post_process.InitFBO(light_pos); // TODO: Move lights into scene
}

void Scene::processInputs(int key)
{
    options.inputReceived(key);
}

void Scene::render(Camera* camera, double time)
{
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    post_process.bindFBO();

    // activate shader
    rend_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    rend_shader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera->GetViewMatrix();
    rend_shader.setMat4("view", view);
    // Bind textures // TODO: MOVE THIS TO POST PROCESS
    // glBindTexture(GL_TEXTURE_2D, water_normals.sampleTexture(glfwGetTime()).texture);
    // Our water normal map covers a 5 x 5 m area
    // rend_shader.setVec2("waterNormalsMapSize", 5, 5);

    draw_models(rend_shader);
    shadowmap.draw_in_map([this] (Shader& s) { draw_models(s); });
    glViewport(0, 0, scr_width, scr_height);

    // After drawing the scene, add the post processing effects
    post_process.renderFBO(options, time, shadowmap, camera->Position);

    // Hot reload shaders
    if (options.reloadShadersNextFrame) {
        options.reloadShadersNextFrame = false;
        reload_shaders();
    }
}

void Scene::draw_models(Shader& shader) {
    for (const auto& scene_object : objects) {
        shader.setMat4("model", scene_object.transform.get_model());
        scene_object.model->Draw(shader);
    }
}

void Scene::reload_shaders() {
    rend_shader.reload();
    post_process.reload_shaders();
    shadowmap.reload_shaders();
}

