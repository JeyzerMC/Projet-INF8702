#version 460 core
layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec4 oColor;

in vec2 vertTexCoord;

// Textures
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gSmooth;
uniform sampler2D caustics;
uniform sampler2D turbulentFlow;
uniform sampler2D pigmentDispersion;
uniform sampler2D paperLayer;
uniform sampler2D abstractColor;
uniform sampler2D shadowMap;

uniform int scr_width;
uniform int scr_height;

uniform mat4 lightMatrix;

uniform vec3 camPos;

uniform vec2 waterNormalsSize;
const float waterRefracitonIndex = 1.333;

// Toggling effects
uniform int showEffects;
uniform bool showToonShading;
uniform bool showCaustics;
uniform bool showWobbling;
uniform bool showEdges;
uniform bool showNormalSmoothing;
uniform bool showWatercolorTextures;

// Light
// TODO: Consider adding multiple light sources.
uniform vec3 lightPos; 
vec3 lightColor = vec3(0.6157, 0.8941, 0.9333);
float ambientStrength = 0.1;
const vec3 clearColor = vec3(0, 0.15, 0.20);

const float epsilon = 0.0001;

float w_offset = 1.0 / scr_width;
float h_offset = 1.0 / scr_height;

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

    vec3 smoothNorm = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        smoothNorm += vec3(texture(gNormal, vertTexCoord.st + offsets[i]));
    }
    smoothNorm /= 9;
    return smoothNorm;
}

float shadowFactor(vec3 pos, vec3 normal)
{
    vec4 shadowMapPosProj = lightMatrix * vec4(pos + normal * 0.1, 1.0);
    vec3 shadowMapPos = shadowMapPosProj.xyz / shadowMapPosProj.w;
    shadowMapPos = shadowMapPos * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, shadowMapPos.xy).r;
    float currentDepth = shadowMapPos.z;
    return currentDepth /*- 0.001*/ < closestDepth  ? 1.0 : 0.0;
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

float applyToonGrading(float intensity) {
    return round(intensity * 3) / 3;
}

vec3 toonShading(vec3 color)
{
    vec3 hsvColor = rgb2hsv(color);
    hsvColor.z = applyToonGrading(hsvColor.z);
    return hsv2rgb(hsvColor);
}

// Takes something in the range [src_low, src_high] and remaps it to the range [dst_low, dst_high]
vec2 remap(vec2 value, vec2 src_low, vec2 src_high, vec2 dst_low, vec2 dst_high) {
    return dst_low + (value - src_low) * (dst_high - dst_low) / (src_high - src_low);
}

vec3 getCaustics(vec3 pos, vec3 normal)
{
    vec2 normalizedPosition = remap(pos.xz, -waterNormalsSize / 2, waterNormalsSize/2, vec2(0), vec2(1));
    vec3 light = texture(caustics, normalizedPosition).rgb;
    float diff = dot(normalize(normal), vec3(0, 1, 0));

    return light * diff;
}

vec3 getEffects(vec3 pos, vec3 normal)
{
    vec3 effects = ambientLight();
    float shadows = shadowFactor(pos, normal);
    effects += diffuseLight(pos, normal) * shadows;
    if (showEffects == 1 || (showEffects == 0 && showCaustics))
        effects += shadows * getCaustics(pos, normal);
    if (showEffects == 1 || (showEffects == 0 && showToonShading))
        effects = toonShading(effects);

    return effects;
}

bool nearWhite(vec3 color)
{
    float threshold = 0.5;
    return length(color) > threshold;
}

vec3 edgeDetection(vec3 litColor, vec2 texCoords)
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

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(gColor, texCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    if (nearWhite(col)) 
        col = vec3(0.0); // Change to make color darker rather than black.
    else
        col = litColor;
    return col;
}

vec2 getPaperGradient() {
    float wobFactor = 0.05;
    float dx = texture(paperLayer, vertTexCoord + vec2(wobFactor, 0)).r - texture(paperLayer, vertTexCoord - vec2(wobFactor, 0)).r;
    float dy = texture(paperLayer, vertTexCoord + vec2(0, wobFactor)).r - texture(paperLayer, vertTexCoord - vec2(0, wobFactor)).r;
    return vec2(dx, dy);
}

void main()
{
    vec2 texCoords = vertTexCoord;
    if (showEffects == 1 || (showEffects == 0 && showWobbling)) {
        texCoords += getPaperGradient() * 0.015;
        // For some reason, 1 still resulted in wrapping, but 0.9999 is fine :/
        texCoords = clamp(texCoords, 0, 0.9999);
    }
    
    vec3 normal;
    if (showEffects == 1 || (showEffects == 0 && showNormalSmoothing))
        // normal = normalSmoothing();
        normal = texture(gSmooth, texCoords).rgb;
    else 
        normal = texture(gNormal, texCoords).rgb;

    if (length(normal) < epsilon) {
        oColor = vec4(clearColor, 1);
        oPosition = vec3(0.0); // TODO: CHECK
        // fragColor = vec4(clearColor, 1);
        return;
    }

    vec3 pos = texture(gPosition, texCoords).rgb;

    vec3 color = texture(gColor, texCoords).rgb;

    vec3 litColor = getEffects(pos, normal) * color;

    // Show regular image
    vec3 col;
    if (showEffects == 1 || (showEffects == 0 && showEdges)) {
        col = edgeDetection(litColor, texCoords);
    } else {
        col = litColor;
    }

    if (showEffects == 1 || (showEffects == 0 && showWatercolorTextures)) {
        col = mix(col, col * texture(turbulentFlow, pos.xz / 15.0).rgb, 0.8);
        col = mix(col, col * texture(pigmentDispersion, pos.xz).rgb, 0.3);
        col = mix(col, col * texture(paperLayer, pos.xz / 5.0).rgb, 0.9);
    }

    oColor = vec4(col, 1.0);
    oPosition = pos;
}