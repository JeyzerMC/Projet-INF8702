//
// Created by arnaud on 2020-11-20.
//

#include "texture.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <stb_image.h>

TextureParameters TextureParameters::get_default() {
    return TextureParameters {
        GL_REPEAT,
        GL_REPEAT,
        GL_LINEAR,
        GL_LINEAR,
        GL_RGB,
    };
}



Texture::Texture(TextureData data, unsigned width, unsigned height, TextureParameters parameters)
    : texture(0)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrap_mode_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrap_mode_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.max_filter);

    glTexImage2D(GL_TEXTURE_2D, 0, parameters.texture_format, width, height, 0, data.data_format, data.data_type, data.data);
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

    auto data = TextureData {
        image_data.get(),
        GL_RGB,
        GL_UNSIGNED_BYTE,
    };

    return Texture(data, x, y, parameters);
}
