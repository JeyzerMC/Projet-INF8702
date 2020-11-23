#version 460 core
layout (location = 0) in vec3 pos;
// layout (location = 1) in vec2 aTexCoord;

// out vec2 TexCoord;
out vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPositionProjective = model * vec4(pos, 1.0f);
    worldPosition = worldPositionProjective.xyz / worldPositionProjective.w;
	gl_Position = projection * view * worldPositionProjective;
	// TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}