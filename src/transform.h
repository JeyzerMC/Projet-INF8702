//
// Created by arnaud on 2020-11-30.
//

#ifndef SCENE_SOUS_MARINE_TRANSFORM_H
#define SCENE_SOUS_MARINE_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    glm::vec3 position;
    glm::quat rotation;
    float scale;

    Transform();
    Transform(glm::vec3 position, glm::quat rotation, float scale);

    [[nodiscard]] glm::mat4 get_model() const;
};


#endif //SCENE_SOUS_MARINE_TRANSFORM_H
