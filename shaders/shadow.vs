#version 460 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 lightMatrix;

uniform bool isAFish;
uniform float time;

void main() {
    vec3 localPos = pos;
    if (isAFish) {
        localPos.x = localPos.x + 0.05 * sin(2 * 3.14 * localPos.z + time * 2);
    }
    gl_Position = lightMatrix * model * vec4(localPos, 1.0);
}
