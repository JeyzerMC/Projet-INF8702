#include "underwater.h"
#include "../utils/constants.h"

Underwater::Underwater()
: uw_shader("shaders/underwater.vs", "shaders/underwater.fs")
, t_turbulent_flow(Texture::load_from_file("textures/perlin_noise.jpg"))
, t_pigment_dispersion(Texture::load_from_file("textures/gaussian_noise.jpg"))
, t_paper_layer(Texture::load_from_file("textures/cotton_paper_2.jpg"))
{
    uw_shader.use();
    uw_shader.setInt("oPosition", 0);
    uw_shader.setInt("oColor", 1);
    uw_shader.setInt("turbulentFlow", 2);
    uw_shader.setInt("pigmentDispersion", 3);
    uw_shader.setInt("paperLayer", 4);

    uw_shader.setInt("scr_width", Constants::SCR_WIDTH);
    uw_shader.setInt("scr_height", Constants::SCR_HEIGHT);
}

void Underwater::render(const Options& options, const glm::vec3& camPos)
{
    clear();

    uw_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_color);

    // Watercolor effects textures
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, t_turbulent_flow.texture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, t_pigment_dispersion.texture);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, t_paper_layer.texture);

    uw_shader.setVec3("camPos", camPos);
    uw_shader.setInt("showEffects", options.showEffects);
    uw_shader.setBool("showBlur", options.showBlur);
    uw_shader.setBool("showTint", options.showTint);
    uw_shader.setBool("showEdges", options.showEdges);
    uw_shader.setBool("showWobbling", options.showWobbling);
    uw_shader.setBool("showWatercolorTextures", options.showWatercolorTextures);

    draw();
}

void Underwater::reload()
{
    uw_shader.reload();
}