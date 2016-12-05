#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in mat4 M;
//layout(location = 5) in vec3 _normal;

uniform mat4 M_common;
uniform mat4 V;
uniform mat4 P;

void main() {
    gl_Position = P * V * M * M_common * vec4(position, 1.0);
}
