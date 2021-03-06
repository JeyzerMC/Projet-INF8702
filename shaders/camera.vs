#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 smoothNormal;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragSmoothNormal;
// out vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isAFish;
uniform float time;

void main()
{
	vec3 localPos = pos;
	if (isAFish) {
		localPos.x = localPos.x + 0.05 * sin(2 * 3.14 * localPos.z + time * 2);
	}
	vec4 worldPositionProjective = model * vec4(localPos, 1.0f);
    // worldPosition = worldPositionProjective.xyz / worldPositionProjective.w;
	gl_Position = projection * view * worldPositionProjective;
	fragPos = worldPositionProjective.xyz / worldPositionProjective.w;
	fragNormal = mat3(model) * normal;
	fragTexCoord = texCoord;
	fragSmoothNormal = mat3(model) * smoothNormal;
}