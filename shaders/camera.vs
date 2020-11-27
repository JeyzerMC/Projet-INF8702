#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
// out vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPositionProjective = model * vec4(pos, 1.0f);
    // worldPosition = worldPositionProjective.xyz / worldPositionProjective.w;
	gl_Position = projection * view * worldPositionProjective;
	fragPos = worldPositionProjective.xyz;
	fragNormal = mat3(model) * normal;
	fragTexCoord = texCoord;
}