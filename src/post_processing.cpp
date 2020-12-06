#include "post_processing.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "../utils/constants.h"

std::vector<arno::Texture> load_water_textures();

PostProcessing::PostProcessing()
    : pp_shader("shaders/post_processing.vs", "shaders/post_processing.fs")
    , caustics(1024, 1024)
    , t_turbulent_flow(arno::Texture::load_from_file("textures/perlin_noise.jpg"))
    , t_pigment_dispersion(arno::Texture::load_from_file("textures/gaussian_noise.jpg"))
    , t_paper_layer(arno::Texture::load_from_file("textures/cotton_paper_2.jpg"))
    , t_abstract_colors(arno::Texture::load_from_file("textures/abstract_colors.jpg"))
    , light_pos(0)
    , erosion_pass()
    , opening_dilation_pass()
    , closing_dilation_pass()
    , uw_pass()
{
    // Screen quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Constants::QUAD_VERTICES), &Constants::QUAD_VERTICES, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    init_shader();
    initBuffers();
    initP2Buffers();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

PostProcessing::~PostProcessing()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void PostProcessing::InitFBO(glm::vec3 lightPos)
{
    light_pos = lightPos;
    init_shader();
}

void PostProcessing::bindFBO()
{
    // The important thing is to have tho normal buffer set to 0, we use that info to see if we rendered anything
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: CHECK IF NEED DEPTH TEST HERE
    // glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
}

void PostProcessing::renderFBO(const Options& options, double time, const Shadowmap& shadow_map, const glm::vec3& camPos)
{
    this->caustics.render(time);
    
    erosion_pass.bind();

    pp_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_color);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, g_smooth);

    // TODO: BIND HERE THE CAUSTICS MAP
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->caustics.caustics_texture.texture);

    // Watercolor effects textures
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, t_turbulent_flow.texture);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, t_pigment_dispersion.texture);

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, t_paper_layer.texture);

    glActiveTexture(GL_TEXTURE8); // TODO: DELETE
    glBindTexture(GL_TEXTURE_2D, t_abstract_colors.texture);

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, shadow_map.texture.texture);

    pp_shader.setVec3("camPos", camPos);
    pp_shader.setMat4("lightMatrix", shadow_map.get_light_matrix());
    pp_shader.setInt("showEffects", options.showEffects);
    pp_shader.setBool("showToonShading", options.showToonShading);
    pp_shader.setBool("showCaustics", options.showCaustics);
    pp_shader.setBool("showWobbling", options.showWobbling);
    pp_shader.setBool("showEdges", options.showEdges);
    pp_shader.setBool("showNormalSmoothing", options.normalSmoothing);
    pp_shader.setBool("showWatercolorTextures", options.showWatercolorTextures);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // Morphological smoothing pass
    opening_dilation_pass.bind();
    erosion_pass.render(options, camPos);

    closing_dilation_pass.bind();
    opening_dilation_pass.render(options, camPos);

    erosion_pass.bind();
    closing_dilation_pass.render(options, camPos);

    uw_pass.bind();
    erosion_pass.render(options, camPos);

    // Final underwater pass
    uw_pass.render(options, camPos);
}

void PostProcessing::initBuffers()
{
    glGenFramebuffers(1, &g_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

    // position color buffer
    glGenTextures(1, &g_position);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Constants::SCR_WIDTH, Constants::SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position, 0);

    // normal color buffer
    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Constants::SCR_WIDTH, Constants::SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal, 0);

    // pixel color buffer
    glGenTextures(1, &g_color);
    glBindTexture(GL_TEXTURE_2D, g_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Constants::SCR_WIDTH, Constants::SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_color, 0);

    // smooth normal buffer
    glGenTextures(1, &g_smooth);
    glBindTexture(GL_TEXTURE_2D, g_smooth); // TODO: CHANGE THE RGBA TO RGB ?
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Constants::SCR_WIDTH, Constants::SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_smooth, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Constants::SCR_WIDTH, Constants::SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
}

void PostProcessing::initP2Buffers()
{
    // TODO: DELETE
}

void PostProcessing::reload_shaders() {
    pp_shader.reload();
    erosion_pass.reload();
    opening_dilation_pass.reload();
    closing_dilation_pass.reload();
    uw_pass.reload();
    init_shader();
    caustics.reload_shaders();
}

void PostProcessing::init_shader() {
    pp_shader.use();
    pp_shader.setInt("screenTexture", 0);
    pp_shader.setInt("scr_width", Constants::SCR_WIDTH);
    pp_shader.setInt("scr_height", Constants::SCR_HEIGHT);

    pp_shader.setInt("gPosition", 0);
    pp_shader.setInt("gNormal", 1);
    pp_shader.setInt("gColor", 2);
    pp_shader.setInt("gSmooth", 3);
    pp_shader.setInt("caustics", 4);
    pp_shader.setInt("turbulentFlow", 5);
    pp_shader.setInt("pigmentDispersion", 6);
    pp_shader.setInt("paperLayer", 7);
    pp_shader.setInt("abstractColor", 8);
    pp_shader.setInt("shadowMap", 9);

    pp_shader.setVec3("lightPos", light_pos);
    pp_shader.setVec2("waterNormalsSize", glm::vec2(15, 15));
}
