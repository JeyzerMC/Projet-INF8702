//
// Created by arnaud on 2020-11-19.
//

#ifndef SCENE_SOUS_MARINE_MODEL_H
#define SCENE_SOUS_MARINE_MODEL_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad.h>
#include <fmt/format.h>

struct Vertex {
    float position[3];
};

class Model {
public:
    Model(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
    ~Model();
    void render();

    static Model load_from_file(const std::string& file_path, const std::string& material_dir);

    GLuint vao;
    GLuint vbo;
    GLuint ibo;

    unsigned int element_count;
};

#endif //SCENE_SOUS_MARINE_MODEL_H
