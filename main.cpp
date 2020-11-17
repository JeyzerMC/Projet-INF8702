#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "utils/shader_reader.h"

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

unsigned int VBO, VAO;

// Shaders
std::unique_ptr<Shader> triangleShader;

void drawTriangle()
{
    // Load Vertex shader
    // std::ifstream fileStream(filePath, std::ios::in);
    // const char* vs_ptr = readShaderSource("shaders/triangle.vs").c_str();
    
    // unsigned int vertexShader;
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vs_ptr, NULL);
    // glCompileShader(vertexShader);
}

void initScene()
{
    // https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp
    triangleShader = std::make_unique<Shader>("shaders/triangle.vs", "shaders/triangle.fs");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
