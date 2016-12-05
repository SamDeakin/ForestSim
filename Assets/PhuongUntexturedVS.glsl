#version 330

out vec3 vertexNormal;
out vec3 vertexPosition;
out vec4 shadowPosition;
out vec3 vertexKd;
out vec3 vertexKs;
out float vertexNs;

layout(location = 0) in vec3 position;
// Note M takes up 4 positions
layout(location = 1) in mat4 M;
layout(location = 5) in vec3 normal;
layout(location = 6) in vec3 Kd;
layout(location = 7) in vec3 Ks;
layout(location = 8) in float Ns;

uniform mat4 M_common;
uniform mat4 V;
uniform mat4 P;

uniform mat4 bias_P_V_shadow;

void main() {
    mat4 modelTransform = M * M_common;

    // For Phuong shading
    mat4 halfTransform = V * modelTransform;
    vertexPosition = normalize((halfTransform * vec4(position, 1.0)).xyz);

    // The actual screen-space position
    mat4 transform = P * halfTransform;
    gl_Position = transform * vec4(position, 1.0);

    // Colour and normal to be interpolated across the surface
    vertexNormal = normalize((transpose(inverse(halfTransform)) * vec4(normal, 0.0)).xyz);

    shadowPosition = bias_P_V_shadow * modelTransform * vec4(position, 1.0);

    // Just pass through the colour values
    vertexKd = Kd;
    vertexKs = Ks;
    vertexNs = Ns;
}
