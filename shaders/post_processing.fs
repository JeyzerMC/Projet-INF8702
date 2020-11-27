#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

uniform sampler2D screenTexture;
uniform bool showEdges;
uniform int scr_width;
uniform int scr_height;


bool nearWhite(vec3 color)
{
    float threshold = 0.5;
    // if (color.x < threshold) return false;
    // if (color.y < threshold) return false;
    // if (color.z < threshold) return false;
    // return true;
    return length(color) > threshold;
}

vec3 edgeDetection()
{
    float w_offset = 1.0 / scr_width;
    float h_offset = 1.0 / scr_height;

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

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, vertTexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    if (nearWhite(col)) 
        col = vec3(0.0);
    else
        col = vec3(1.0);
    return col;
}

void main()
{
    // Show regular image
    vec3 col;

    if (showEdges) {
        col = edgeDetection();
    } else {
       col = texture(screenTexture, vertTexCoord).rgb;
    }

    fragColor = vec4(col, 1.0);
} 