#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

uniform sampler2D screenTexture;
uniform bool showEdges;

const float offset = 1.0 / 300.0;

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
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1,-9, 1,
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