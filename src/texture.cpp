//
// Created by arnaud on 2020-11-20.
//

#include "texture.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <stb_image.h>

namespace arno {
TextureParameters TextureParameters::get_default() {
    return TextureParameters {
        GL_REPEAT,
        GL_REPEAT,
    };
}



Texture::Texture(unsigned char *data, int width, int height, TextureParameters parameters)
    : texture(0)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrap_mode_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrap_mode_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

Texture::Texture(Texture&& other) noexcept
    : texture(other.texture)
{
    other.texture = 0;
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

Texture Texture::load_from_file(const std::string &file_path, TextureParameters parameters) {
    int x, y, n;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image_data(
            stbi_load(file_path.c_str(), &x, &y, &n, 3),
            stbi_image_free);

    if (image_data == nullptr) {
        spdlog::error("image load failed: {}", stbi_failure_reason());
        throw std::runtime_error("Failed to load image.");
    }

    if (n != 3) {
        throw std::runtime_error(fmt::format("image {} had {} channels, it must have 3", file_path, n));
    }

    return Texture(image_data.get(), x, y, parameters);
}
}
