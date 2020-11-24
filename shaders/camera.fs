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

void main()
{
    // BEFORE:
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    // CAUSTICS: 
    // vec3 normal = normalize(texture(waterNormalMap, worldPosition.xz/4).rgb);
    // vec3 refractedRay = vec3(0, 0, 1);
    // vec3 incidentRay = refract(refractedRay, -normal, waterRefracitonIndex);
    // float cosAngle = dot(incidentRay, vec3(0, 0, 1));
    // float light = exp(-100 * (cosAngle - 1) * (cosAngle - 1));

    // fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f) * light;
    fragColor = vec4(vertColor.xyz, 1.0f);
}