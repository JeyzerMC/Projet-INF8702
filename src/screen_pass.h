#ifndef SCREEN_PASS_H
#define SCREEN_PASS_H
#include <glad/glad.h>
#include "../utils/shader.h"
#include "../utils/options.h"

#include <vector>

class ScreenPass {

public:
    ScreenPass();
    ~ScreenPass();

    virtual void bind();
    virtual void draw();
    virtual void clear();
    // virtual void reload() = 0;

protected:
    virtual void initBuffers();
    
    unsigned int VAO, VBO;
    unsigned int g_buffer;
    unsigned int g_position, g_color;
};

#endif //SCREEN_PASS_H