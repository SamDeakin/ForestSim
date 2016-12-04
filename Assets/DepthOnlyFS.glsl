#version 330 core

out vec4 colour;
layout(location = 1) out float depth;

void main() {
    depth = gl_FragCoord.z;

    // We do this to facilitate showing the depth texture on screen
    colour = vec4(vec3(gl_FragCoord.z), 1.0);
}
