#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, vertTexCoord).rgb;
    fragColor = vec4(col, 1.0);
} 