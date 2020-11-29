#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;
layout (location = 3) out vec3 gSmoothNormal;
// out vec4 fragColor;

// in vec2 TexCoord;
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragSmoothNormal;
// in vec3 worldPosition;

// Texture samplers
uniform sampler2D texture_diffuse1;

void main()
{
    // vec4 vertColor = texture(texture_diffuse1, vertTexCoord);
    // vec3 finalColor = getEffects() * vertColor.xyz;
    // fragColor = vec4(finalColor, 1.0f);
    gPosition = fragPos;
    gNormal = normalize(fragNormal);
    gColor = texture(texture_diffuse1, fragTexCoord);
    gSmoothNormal = normalize(fragSmoothNormal);
} 