#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
