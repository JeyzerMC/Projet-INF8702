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

// Toggling effects
uniform int showEffects;
uniform bool showDilation;

float w_offset = 1.0 / scr_width;
float h_offset = 1.0 / scr_height;

vec3 dilate(vec3 col)
{
    vec2 offsets[9] = vec2[](
        vec2(-w_offset,  h_offset), // top-left
        vec2( 0.0f,    h_offset), // top-center
        vec2( w_offset,  h_offset), // top-right
        vec2(-w_offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( w_offset,  0.0f),   // center-right
        vec2(-w_offset, -h_offset), // bottom-left
        vec2( 0.0f,   -h_offset), // bottom-center
        vec2( w_offset, -h_offset)  // bottom-right    
    );

    float r = col.r;
    float g = col.g; 
    float b = col.b;
    for (int i = 0; i < 9; i++)
    {
        vec3 sampleTex = texture(oColor, vertTexCoord + offsets[i]).rgb;
        if (sampleTex.r > r) r = sampleTex.r;
        if (sampleTex.g > g) g = sampleTex.g;
        if (sampleTex.b > b) b = sampleTex.b;
    }

    return vec3(r, g, b);
}

void main()
{
    vec3 col = texture(oColor, vertTexCoord).rgb;
    gPosition = texture(oPosition, vertTexCoord).rgb;

    if (showEffects == 1 || (showEffects == 0 && showDilation))
    {
        gColor = vec4(dilate(col), 1.0);
        // gColor = vec4(1.0, 1.0, 0.0, 1.0);
    }
    else
    {
        gColor = vec4(col, 1.0);
    }
}