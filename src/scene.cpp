#include "scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    fish(make_shared<Model>("models/Fishes/ClownFish.obj")),
    objects(),
    post_process(w, h),
    shadowmap(1024, 1024),
    light_pos(10.0, 50.0, 10.0)
// water_normals(), // TODO: RE-ADD CAUSTICS
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
        Transform(glm::vec3(4, 0, 4), glm::quat(glm::vec3(0, 0, 0)), 0.15),
        pot,
    });

    objects.push_back(SceneObject{
            Transform(glm::vec3(0, 3, 0), glm::quat(glm::vec3(0, 0, 0)), 2),
            fish,
    });

//    water_normals.loop_mode = LoopMode::PingPong;
    glEnable(GL_DEPTH_TEST); // TODO: CHECK IF STAYS HERE
    post_process.InitFBO(light_pos); // TODO: Move lights into scene
}

void Scene::render(Camera* camera, bool toonShading, bool caustics, bool showWobbling, int edges, int smoothLevel, double time)
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
    post_process.renderFBO(toonShading, caustics, showWobbling, edges, smoothLevel, time, shadowmap);
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

