#version 460 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;

out vec2 vertTexCoord;

void main()
{
    vertTexCoord = texCoord;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
}  