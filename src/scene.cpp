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
    ground("models/Ground/Ground.obj", false),
    pot("models/Pot/Pot.obj"),
    post_process(w, h),
    light_pos(10.0, 50.0, 10.0)
// water_normals(), // TODO: RE-ADD CAUSTICS
{
//    water_normals.loop_mode = LoopMode::PingPong;
    glEnable(GL_DEPTH_TEST); // TODO: CHECK IF STAYS HERE
    post_process.InitFBO(light_pos); // TODO: Move lights into scene
}

void Scene::render(Camera* camera, bool toonShading, bool caustics, int edges, int smoothLevel, double time)
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

    // Ground
    auto model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(0, -1, 0));
    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
    rend_shader.setMat4("model", model);
    ground.Draw(rend_shader);

    // Pot 1
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(-2, 0, -2));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    rend_shader.setMat4("model", model);
    pot.Draw(rend_shader);

    // Pot 2
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(4, 0, 4));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.15, 0.15, 0.15));
    rend_shader.setMat4("model", model);
    pot.Draw(rend_shader);

    // After drawing the scene, add the post processing effects
    post_process.renderFBO(toonShading, caustics, edges, smoothLevel, time);
}

