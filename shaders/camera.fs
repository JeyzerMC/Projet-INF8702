#version 460 core
out vec4 fragColor;

// in vec2 TexCoord;
in vec3 worldPosition;
in vec3 vertColor;

// texture samplers
// uniform sampler2D texture1;
// uniform sampler2D texture2;
uniform sampler2D waterNormalMap;

const float waterRefracitonIndex = 1.333;

// Utility functions to change from rgb to hsv and opposite.
// Comes from: https://gist.github.com/yiwenl
vec3 rgb2hsv(vec3 rgb) {
    float Cmax = max(rgb.r, max(rgb.g, rgb.b));
    float Cmin = min(rgb.r, min(rgb.g, rgb.b));
    float delta = Cmax - Cmin;

    vec3 hsv = vec3(0., 0., Cmax);

    if (Cmax > Cmin) {
        hsv.y = delta / Cmax;

        if (rgb.r == Cmax)
            hsv.x = (rgb.g - rgb.b) / delta;
        else {
            if (rgb.g == Cmax)
                hsv.x = 2. + (rgb.b - rgb.r) / delta;
            else
                hsv.x = 4. + (rgb.r - rgb.g) / delta;
        }
        hsv.x = fract(hsv.x / 6.);
    }
    return hsv;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void toonShading()
{

}

void main()
{
    // BEFORE:
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    // CAUSTICS: 
     vec3 normal = normalize(texture(waterNormalMap, worldPosition.xz/4).rgb);
     vec3 refractedRay = vec3(0, 0, 1);
     vec3 incidentRay = refract(refractedRay, -normal, waterRefracitonIndex);
     float cosAngle = dot(incidentRay, vec3(0, 0, 1));
     float light = exp(-100 * (cosAngle - 1) * (cosAngle - 1));

    // fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f) * light;
    fragColor = vec4(vertColor.xyz, 1.0f) + light;
}