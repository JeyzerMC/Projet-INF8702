#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
// layout (location = 1) in vec2 aTexCoord;

// out vec2 TexCoord;
out vec3 worldPosition;
out vec3 vertNormal;
out vec2 vertTexCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPositionProjective = model * vec4(pos, 1.0f);
    worldPosition = worldPositionProjective.xyz / worldPositionProjective.w;
	gl_Position = projection * view * worldPositionProjective;
	vertNormal = mat3(model) * normal;
	vertTexCoord = texCoord;
	fragPos = worldPositionProjective.xyz;
}