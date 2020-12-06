//
// Created by arnaud on 2020-11-20.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glad/glad.h>
struct TextureParameters {
    GLint wrap_mode_s;
    GLint wrap_mode_t;

    GLint min_filter;
    GLint max_filter;

    GLint texture_format;

    static TextureParameters get_default();
};

struct TextureData {
    unsigned char* data;
    GLint data_format;
    GLint data_type;
};

class Texture {
public:
    Texture(TextureData data, unsigned width, unsigned height, TextureParameters parameters);
    Texture(const Texture&) = delete;
    Texture(Texture&&) noexcept;
    ~Texture();
    static Texture load_from_file(const std::string& file_path, TextureParameters parameters = TextureParameters::get_default());

    GLuint texture;
};


#endif //TEXTURE_H
