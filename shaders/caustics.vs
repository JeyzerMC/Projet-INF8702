#version 460 core

layout (location = 0) in vec3 pos;

out vec3 old_position;
out vec3 new_position;

const float water_depth = 1.5;

float waterHeight(vec2 pos) {
    return 0.1 * (sin(20 * pos.x) + sin(50 * pos.y));
}

void main() {
    float height = waterHeight(pos.xy);
    float dx = waterHeight(pos.xy + vec2(0.01, 0)) - waterHeight(pos.xy - vec2(0.01, 0));
    float dy = waterHeight(pos.xy + vec2(0, 0.01)) - waterHeight(pos.xy - vec2(0, 0.01));
    vec3 normal = normalize(vec3(dx, dy, 1));
    vec3 sun_ray = vec3(0, 0, -1);
    vec3 refracted_ray = refract(sun_ray, normal, 1.33);

    old_position = pos;
    new_position = old_position + vec3(refracted_ray.xy, 0) * (water_depth + height);
    gl_Position = vec4(new_position, 1.0);
}
