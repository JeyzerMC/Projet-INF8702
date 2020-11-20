//
// Created by arnaud on 2020-11-19.
//

#include "model.h"

#include <algorithm>

#include <tiny_obj_loader.h>
#include <spdlog/spdlog.h>

Model::Model(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    : vao(0)
    , vbo(0)
    , ibo(0)
    , element_count(indices.size())
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
}



Model::~Model() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}



Model Model::loadFromFile(const std::string &file_path, const std::string& material_dir) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    auto result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str(), material_dir.c_str(), true);

    if (!warn.empty()) {
        spdlog::warn(warn);
    }
    if (!err.empty()) {
        spdlog::error(err);
    }
    if (!result) {
        throw std::runtime_error("Failed to load obj file.");
    }

    auto vertices_number = attrib.vertices.size() / 3;
    std::vector<Vertex> vertices;
    vertices.reserve(vertices_number);
    for (unsigned i = 0; i < vertices_number; ++i) {
        auto x = attrib.vertices[i * 3 + 0];
        auto y = attrib.vertices[i * 3 + 1];
        auto z = attrib.vertices[i * 3 + 2];

        auto v = Vertex();
        v.position[0] = x;
        v.position[1] = y;
        v.position[2] = z;
        vertices.push_back(v);
    }

    if (shapes.empty()) {
        spdlog::error("Cannot load model `{}`, no shape inside.", file_path);
        throw std::runtime_error("Failed to load obj file.");
    }
    if (shapes.size() != 1) {
        spdlog::warn("Model `{}` contains {} shapes, but the loader can only load one shape. Only the first shape will be loaded.", file_path, shapes.size());
    }

    std::vector<unsigned> indices;
    indices.resize(shapes[0].mesh.indices.size());
    std::transform(shapes[0].mesh.indices.begin(), shapes[0].mesh.indices.end(), indices.begin(), [](tinyobj::index_t i) {
        return i.vertex_index;
    });

    return Model(vertices, indices);
}


