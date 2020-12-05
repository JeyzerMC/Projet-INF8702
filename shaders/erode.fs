#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gColor;
// out vec4 fragColor;


in vec2 vertTexCoord;

// Textures
uniform sampler2D oPosition;
uniform sampler2D oColor;

uniform int scr_width;
uniform int scr_height;

void main()
{
    vec3 col = texture(oColor, vertTexCoord).rgb;
    gPosition = texture(oPosition, vertTexCoord).rgb;
    gColor = vec4(col, 1.0);
}