#version 460 core
out vec4 fragColor;

// in vec2 TexCoord;
in vec3 worldPosition;
in vec3 vertNormal;
in vec2 vertTexCoord;
in vec3 fragPos;

// Texture samplers
// uniform sampler2D texture2;
uniform sampler2D waterNormalMap;
uniform sampler2D texture_diffuse1;
uniform vec2 waterNormalsMapSize;

// Toggling effects
uniform bool showToonShading;
uniform bool showCaustics;

uniform vec3 lightPos;
vec3 lightColor = vec3(1.0, 1.0, 1.0);
float ambientStrength = 0.1;

const float waterRefracitonIndex = 1.333;

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

vec3 ambientLight()
{
    return ambientStrength * lightColor;
}

vec3 diffuseLight()
{
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

float getToonFactor()
{
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float u = dot(norm, lightDir);
    if (u < 0.5) return 0;
    if (u < 0.7) return 0.2;
    return 1;
}

vec3 toonShading()
{
    vec3 lightHsv = rgb2hsv(lightColor);
    lightHsv.z = lightHsv.z * getToonFactor();
    return hsv2rgb(lightHsv);
}

float getCaustics() {
    vec3 normal = normalize(texture(waterNormalMap, worldPosition.xz/waterNormalsMapSize).rgb);
    vec3 refractedRay = vec3(0, 0, 1);
    vec3 incidentRay = refract(refractedRay, -normal, waterRefracitonIndex);
    float cosAngle = dot(incidentRay, vec3(0, 0, 1));
    float light = exp(-100 * (cosAngle - 1) * (cosAngle - 1));

    float diff = dot(normalize(vertNormal), vec3(0, 0, 1));

    return light * diff;
}

vec3 getEffects()
{
    vec3 effects = ambientLight();
    effects += showToonShading? toonShading(): diffuseLight();

    if (showCaustics)
        effects += getCaustics();
    return effects;
}

void main()
{
    vec4 vertColor = texture(texture_diffuse1, vertTexCoord);
    vec3 finalColor = getEffects() * vertColor.xyz;
    fragColor = vec4(finalColor, 1.0f);
}