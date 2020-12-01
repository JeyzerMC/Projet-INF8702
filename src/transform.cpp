//
// Created by arnaud on 2020-11-30.
//

#include "transform.h"

Transform::Transform()
    : position(glm::vec3(0, 0, 0))
    , rotation(glm::vec3(0, 0, 0))
    , scale(1)
{
}

Transform::Transform(glm::vec3 position, glm::quat rotation, float scale)
    : position(position)
    , rotation(rotation)
    , scale(scale)
{
}

glm::mat4 Transform::get_model() const {
    auto scale_matrix = glm::scale(glm::mat4(1), glm::vec3(scale));
    auto rotation_matrix = glm::mat4_cast(rotation);
    auto translation_matrix = glm::translate(glm::mat4(1), position);
    return translation_matrix * rotation_matrix * scale_matrix;
}
