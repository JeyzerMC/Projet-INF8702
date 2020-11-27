#version 460 core
// layout (location = 0) out vec3 gPosition;
// layout (location = 1) out vec3 gNormal;
// layout (location = 2) out vec4 gColor;
out vec4 fragColor;

// in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
// in vec3 worldPosition;

// Texture samplers
uniform sampler2D texture_diffuse1;
// uniform sampler2D waterNormalMap;
// uniform vec2 waterNormalsMapSize;

// const float waterRefracitonIndex = 1.333;

// float getCaustics() {
//     vec3 normal = normalize(texture(waterNormalMap, worldPosition.xz/waterNormalsMapSize).rgb);
//     vec3 refractedRay = vec3(0, 0, 1);
//     vec3 incidentRay = refract(refractedRay, -normal, waterRefracitonIndex);
//     float cosAngle = dot(incidentRay, vec3(0, 0, 1));
//     float light = exp(-100 * (cosAngle - 1) * (cosAngle - 1));

//     float diff = dot(normalize(vertNormal), vec3(0, 0, 1));

//     return light * diff;
// }

void main()
{
    // vec4 vertColor = texture(texture_diffuse1, vertTexCoord);
    // vec3 finalColor = getEffects() * vertColor.xyz;
    // fragColor = vec4(finalColor, 1.0f);

    // gPosition = fragPos;
    // gNormal = normalize(fragNormal);
    // // gColor = texture(texture_diffuse1, fragTexCoord);
    // gColor = vec4(1.0, 0.0, 0.0, 1.0);
    fragColor = vec4(0.0, 1.0, 0.0, 1.0);
} 