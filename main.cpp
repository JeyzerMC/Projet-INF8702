#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "src/model.h"
#include "src/texture.h"
#include "src/animated_texture.h"
#include "src/post_processing.h"

#include "utils/shader.h"
#include "utils/camera.h"
#include "utils/model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

bool debug = true;
bool showToonShading = true;
bool showCaustics = true;

glm::vec3 lightPosition(30.0, 30.0, 50.0);

std::vector<arno::Texture> load_water_textures() {
    int min_index = 1;
    int max_index = 250;
    auto vector = std::vector<arno::Texture>();
    vector.reserve(max_index - min_index + 1);
    spdlog::info("Loading water normals. This will take some seconds.");
    for (int i = min_index; i <= max_index; ++i) {
        auto file_path = fmt::format("textures/water_height/{:04}.png", i);
        vector.push_back(arno::Texture::load_from_file(file_path));
    }
    spdlog::info("Finished loading water normals.");
    return vector;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GL_TRUE : GL_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize GLAD");
        return -1;
    }
    spdlog::info("Using Opengl {}.{}", GLVersion.major, GLVersion.minor);
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader baseShader("shaders/camera.vs", "shaders/camera.fs");
    Model bp_model("models/Backpack.obj");
    PostProcessing post_processing(SCR_WIDTH, SCR_HEIGHT);

    // Load our custom model
    // auto pirate_model = Model::load_from_file("models/pirate.obj", "models");
    // auto shell_model = Model::load_from_file("models/Pot.obj", "models/");
    // auto cube_model = Cube();
    auto water_normals = AnimatedTexture(load_water_textures(), 24);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    baseShader.use();
    baseShader.setVec3("lightPos", lightPosition);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        post_processing.bindFBO();

        // activate shader
        baseShader.use();
        // Should we render toon shading
        baseShader.setBool("showToonShading", showToonShading);
        // // Should we render caustics
        baseShader.setBool("showCaustics", showCaustics);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        baseShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        baseShader.setMat4("view", view);
        // Bind textures
        glBindTexture(GL_TEXTURE_2D, water_normals.sampleTexture(glfwGetTime()).texture);
        // Our water normal map covers a 5 x 5 m area
        baseShader.setVec2("waterNormalsMapSize", 5, 5);

        // render boxes

        // render our custom model
        auto model = glm::identity<glm::mat4>();
        baseShader.setMat4("model", model);
        bp_model.Draw(baseShader);

        post_processing.renderFBO();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        showToonShading = !showToonShading;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        showCaustics = !showCaustics;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void debug_message_callback(
        GLenum,
        GLenum,
        GLuint,
        GLenum severity,
        GLsizei gl_message_length,
        const GLchar *gl_message,
        const void *) {

    spdlog::level::level_enum level;
    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            level = spdlog::level::debug;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            level = spdlog::level::info;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            level = spdlog::level::warn;
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            level = spdlog::level::err;
            break;
        default:
            level = spdlog::level::err;
    }

    std::string message;
    if (gl_message_length < 0) {
        message = std::string(gl_message);
    }
    else {
        message = std::string(gl_message, gl_message_length);
    }

    spdlog::log(level, message);
}
