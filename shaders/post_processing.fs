#version 460 core
out vec4 fragColor;

in vec2 vertTexCoord;

// Textures
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
// TODO: ADD CAUSTICS HERE

uniform int scr_width;
uniform int scr_height;

// Toggling effects
uniform bool showToonShading;
uniform bool showCaustics;
uniform int showEdges;

// Light
// TODO: Consider adding multiple light sources.
uniform vec3 lightPos; 
vec3 lightColor = vec3(0.6157, 0.8941, 0.9333);
float ambientStrength = 0.1;

// Utility functions to change from rgb to hsv and opposite.
// Comes from: https://gist.github.com/yiwenl
vec3 rgb2hsv(vec3 rgb) {
    float Cmax = max(rgb.r, max(rgb.g, rgb.b));
    float Cmin = min(rgb.r, min(rgb.g, rgb.b));
    float delta = Cmax - Cmin;

    vec3 hsv = vec3(0.0, 0.0, Cmax);

    if (Cmax > Cmin) {
        hsv.y = delta / Cmax;

        if (rgb.r == Cmax)
            hsv.x = (rgb.g - rgb.b) / delta;
        else {
            if (rgb.g == Cmax)
                hsv.x = 2.0 + (rgb.b - rgb.r) / delta;
            else
                hsv.x = 4.0 + (rgb.r - rgb.g) / delta;
        }
        hsv.x = fract(hsv.x / 6.0);
    }
    return hsv;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 normalSmoothing()
{
    return vec3(0.0);
}

vec3 ambientLight()
{
    return ambientStrength * lightColor;
}

vec3 diffuseLight(vec3 pos, vec3 normal)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);  
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

float getToonFactor(vec3 pos, vec3 normal)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);  
    float u = dot(norm, lightDir);
    if (u < 0.5) return 0;
    if (u < 0.7) return 0.2;
    return 1.0;
}

vec3 toonShading(vec3 pos, vec3 normal)
{
    vec3 lightHsv = rgb2hsv(lightColor);
    lightHsv.z = lightHsv.z * getToonFactor(pos, normal);
    return hsv2rgb(lightHsv);
}

vec3 getEffects(vec3 pos, vec3 normal)
{
    vec3 effects = ambientLight();
    effects += showToonShading? toonShading(pos, normal): diffuseLight(pos, normal);

    // if (showCaustics)
    //     effects += getCaustics();
    return effects;
}

bool nearWhite(vec3 color)
{
    float threshold = 0.5;
    return length(color) > threshold;
}

vec3 edgeDetection(vec3 litColor)
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
        sampleTex[i] = vec3(texture(gColor, vertTexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    if (nearWhite(col)) 
        col = vec3(0.0);
    else
        col = showEdges == 0? litColor: vec3(1.0);
    return col;
}

void main()
{

    vec3 pos = texture(gPosition, vertTexCoord).rgb;
    vec3 normal = texture(gNormal, vertTexCoord).rgb;
    vec3 color = texture(gColor, vertTexCoord).rgb;

    vec3 litColor = getEffects(pos, normal) * color;

    // Show regular image
    vec3 col;
    if (showEdges == 2) {
        col = litColor;
    } else {
        col = edgeDetection(litColor);
    }

    fragColor = vec4(col, 1.0);
} 