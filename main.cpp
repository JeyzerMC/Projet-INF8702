#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

int main() {
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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
