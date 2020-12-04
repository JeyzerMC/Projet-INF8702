#include "underwater.h"

Underwater::Underwater(int scrWidth, int scrHeight)
: uw_shader("shaders/underwater.vs", "shaders/underwater.fs")
, scr_width(scrWidth), scr_height(scrHeight)
{
    // Screen quad
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

Underwater::~Underwater()
{

}

void Underwater::render(const Options& options)
{

}

void Underwater::bind()
{
    
}

void Underwater::reload()
{
    uw_shader.reload();
}