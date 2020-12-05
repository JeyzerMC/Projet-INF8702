#include "underwater.h"
#include "../utils/constants.h"

Underwater::Underwater()
: uw_shader("shaders/underwater.vs", "shaders/underwater.fs")
{
    uw_shader.use();
    uw_shader.setInt("oPosition", 0);
    uw_shader.setInt("oColor", 1);

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

    uw_shader.setVec3("camPos", camPos);
    uw_shader.setInt("showEffects", options.showEffects);
    uw_shader.setBool("showBlur", options.showBlur);
    uw_shader.setBool("showTint", options.showTint);

    draw();
}

void Underwater::reload()
{
    uw_shader.reload();
}