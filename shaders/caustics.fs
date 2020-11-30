#version 460 core

in vec3 old_position;
in vec3 new_position;
in float height;

out vec3 color;
void main() {
    float old_area = length(dFdx(old_position)) * length(dFdy(old_position));
    float new_area = length(dFdx(new_position)) * length(dFdy(new_position));
    color = vec3(old_area / new_area) * 0.2;
//    color = vec3(height);
}
