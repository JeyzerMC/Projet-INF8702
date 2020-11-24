//
// Created by arnaud on 2020-11-20.
//

#ifndef SCENE_SOUS_MARINE_TEXTURE_H
#define SCENE_SOUS_MARINE_TEXTURE_H

#include <string>

#include <glad/glad.h>
namespace arno {
struct TextureParameters {
    GLint wrap_mode_s;
    GLint wrap_mode_t;

    static TextureParameters get_default();
};

class Texture {
public:
    Texture(unsigned char* data, int width, int height, TextureParameters parameters);
    Texture(const Texture&) = delete;
    Texture(Texture&&) noexcept;
    ~Texture();
    static Texture load_from_file(const std::string& file_path, TextureParameters parameters = TextureParameters::get_default());

    GLuint texture;
};
}


#endif //SCENE_SOUS_MARINE_TEXTURE_H
