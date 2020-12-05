#include "dilation.h"
#include "../utils/constants.h"

Dilation::Dilation()
: dilate_shader("shaders/dilate.vs", "shaders/dilate.fs")
{
    dilate_shader.use();
    dilate_shader.setInt("oPosition", 0);
    dilate_shader.setInt("oColor", 1);

    dilate_shader.setInt("scr_width", Constants::SCR_WIDTH);
    dilate_shader.setInt("scr_height", Constants::SCR_HEIGHT);
}

void Dilation::render(const Options& options, const glm::vec3& camPos)
{
    // clear();

    dilate_shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_color);

    // dilate_shader.setVec3("camPos", camPos);
    // dilate_shader.setInt("showEffects", options.showEffects);
    // dilate_shader.setBool("showBlur", options.showBlur);
    // dilate_shader.setBool("showTint", options.showTint);

    draw();
}

void Dilation::reload()
{
    dilate_shader.reload();
}