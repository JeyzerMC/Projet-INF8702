#include "erosion.h"
#include "../utils/constants.h"

Erosion::Erosion()
: erode_shader("shaders/erode.vs", "shaders/erode.fs")
{
    erode_shader.use();
    erode_shader.setInt("oPosition", 0);
    erode_shader.setInt("oColor", 1);

    erode_shader.setInt("scr_width", Constants::SCR_WIDTH);
    erode_shader.setInt("scr_height", Constants::SCR_HEIGHT);
}

void Erosion::render(const Options& options, const glm::vec3& camPos)
{
    erode_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_color);

    erode_shader.setInt("showEffects", options.showEffects);
    erode_shader.setBool("showErosion", options.showErosion);

    draw();
}

void Erosion::reload()
{
    erode_shader.reload();
}