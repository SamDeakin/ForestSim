#version 330

out vec3 vertexNormal;
out vec3 vertexPosition;

layout(location = 0) in vec3 colour;
layout(location = 1) in vec3 position;
layout(location = 2) in vec3 normal;
// Note M takes up 4 positions
layout(location = 3) in mat4 M;

uniform mat4 M_common;
uniform mat4 V;
uniform mat4 P;

void main() {
    // For Phuong shading
    mat4 halfTransform = V * M * M_common;
    vertexPosition = normalize((halfTransform * vec4(position, 1.0)).xyz);

    // The actual screen-space position
    mat4 transform = P * halfTransform;
    gl_Position = transform * vec4(position, 1.0);

    // Colour and normal to be interpolated across the surface
    vertexNormal = normalize((transpose(inverse(halfTransform)) * vec4(normal, 0.0)).xyz);
}
