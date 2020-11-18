#define TINYOBJLOADER_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include "utils/shader.h"

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};  

unsigned int VBO, VAO, IBO;

// Shaders
std::unique_ptr<Shader> triangleShader;

// Pirate
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

void loadPirate()
{
    std::string inputfile = "models/pirate.obj";

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), "models", true);

    if (!warn.empty()) {
    std::cout << warn << std::endl;
    }

    if (!err.empty()) {
    std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }
}

void initScene()
{
    loadPirate();
    // https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp
    triangleShader = std::make_unique<Shader>("shaders/triangle.vs", "shaders/triangle.fs");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(tinyobj::real_t) * attrib.vertices.size(), 
                 attrib.vertices.data(), 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(tinyobj::index_t) * shapes[0].mesh.indices.size(),
        shapes[0].mesh.indices.data(),
        GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    triangleShader->use();
}

int main() {
    spdlog::set_level(spdlog::level::debug);

    if (!glfwInit()) {
        spdlog::error("GLFW3 initialization failed");
        return -1;
    }

    auto window = glfwCreateWindow(640, 480, "Submarine scene", nullptr, nullptr);
    if (!window) {
        spdlog::error("GLFW3 window creation failed");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // We want to use OpenGl 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        spdlog::error("{}", glewGetErrorString(glewInitResult));
        glfwTerminate();
        return -1;
    }
    spdlog::info("Using GLEW {}", glewGetString(GLEW_VERSION));

    spdlog::debug("Latest supported OpenGL version: {}", glGetString(GL_VERSION));
    spdlog::debug("Using graphics driver: {}", glGetString(GL_RENDERER));

    if (glewIsSupported("GL_VERSION_4_6")) {
        spdlog::info("OpenGl 4.6 is supported");
    }
    else {
        spdlog::error("OpenGl 4.6 is not supported on this device.");
        glfwTerminate();
        return -1;
    }

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    spdlog::debug("GL_MAX_TEXTURE_UNITS = {}", maxTextureUnits);

    GLint maxVaryingFloats;
    glGetIntegerv(GL_MAX_VARYING_FLOATS, &maxVaryingFloats);
    spdlog::debug("GL_MAX_VARYING_FLOATS = {}", maxVaryingFloats);

    initScene();

    while (!glfwWindowShouldClose(window))
    {
        // Input and clear
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        // Swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
